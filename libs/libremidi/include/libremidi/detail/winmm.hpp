#pragma once
#define NOMINMAX 1
#define WIN32_LEAN_AND_MEAN 1
#include <windows.h>

#include <algorithm>
#include <chrono>
#include <condition_variable>
#include <mmsystem.h>
#include <mutex>
#include <ostream>
#include <libremidi/detail/midi_api.hpp>
#include <libremidi/libremidi.hpp>
#include <sstream>
#include <thread>

// Default for Windows is to add an identifier to the port names; this
// flag can be defined (e.g. in your project file) to disable this behaviour.
//#define LIBREMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES

//*********************************************************************//
//  API: Windows Multimedia Library (MM)
//*********************************************************************//

// API information deciphered from:
//  -
//  http://msdn.microsoft.com/library/default.asp?url=/library/en-us/multimed/htm/_win32_midi_reference.asp

// Thanks to Jean-Baptiste Berruchon for the sysex code.
namespace libremidi
{

#define RT_WINMM_OBSERVER_POLL_PERIOD_MS 100

#define RT_SYSEX_BUFFER_SIZE 1024
#define RT_SYSEX_BUFFER_COUNT 4

// A structure to hold variables related to the WinMM API
// implementation.
struct WinMidiData
{
  HMIDIIN inHandle;   // Handle to Midi Input Device
  HMIDIOUT outHandle; // Handle to Midi Output Device
  DWORD lastTime;
  libremidi::message message;
  LPMIDIHDR sysexBuffer[RT_SYSEX_BUFFER_COUNT];
  CRITICAL_SECTION
  _mutex; // [Patrice] see
          // https://groups.google.com/forum/#!topic/mididev/6OUjHutMpEo
};

// The Windows MM API is based on the use of a callback function for
// MIDI input.  We convert the system specific time stamps to delta
// time values.

// Convert a nullptr-terminated wide string or ANSI-encoded string to UTF-8.
inline std::string ConvertToUTF8(const TCHAR* str)
{
  std::string u8str;
  const WCHAR* wstr = L"";
#if defined(UNICODE) || defined(_UNICODE)
  wstr = str;
#else
  // Convert from ANSI encoding to wide string
  int wlength = MultiByteToWideChar(CP_ACP, 0, str, -1, nullptr, 0);
  std::wstring wstrtemp;
  if (wlength)
  {
    wstrtemp.assign(wlength - 1, 0);
    MultiByteToWideChar(CP_ACP, 0, str, -1, &wstrtemp[0], wlength);
    wstr = &wstrtemp[0];
  }
#endif
  // Convert from wide string to UTF-8
  int length = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, nullptr, 0, nullptr, nullptr);
  if (length)
  {
    u8str.assign(length - 1, 0);
    length = WideCharToMultiByte(CP_UTF8, 0, wstr, -1, &u8str[0], length, nullptr, nullptr);
  }
  return u8str;
}

class observer_winmm final : public observer_api
{
private:
  using CallbackFunc = std::function<void(int, std::string)>;

  std::thread watchThread;
  std::condition_variable watchThreadCV;
  std::mutex watchThreadMutex;
  bool watchThreadShutdown;

  inline static const bool INPUT = true;
  inline static const bool OUTPUT = false;

public:
  using PortList = std::vector<std::string>;

  PortList inputPortList;
  PortList outputPortList;
  observer_winmm(observer::callbacks&& c) : observer_api{std::move(c)}
  {
    inputPortList = get_port_list(INPUT);
    outputPortList = get_port_list(OUTPUT);

    watchThreadShutdown = false;
    watchThread = std::thread([this]() { watch_thread(); });
  }

  ~observer_winmm()
  {
    signal_watch_thread_shutdown();
    watchThreadCV.notify_all();
    if (watchThread.joinable())
      watchThread.join();
  }

private:
  void watch_thread()
  {
    while (!wait_for_watch_thread_shutdown_signal(RT_WINMM_OBSERVER_POLL_PERIOD_MS))
    {
      auto currInputPortList = get_port_list(INPUT);
      compare_port_lists_and_notify_clients(
          inputPortList, currInputPortList, callbacks_.input_added, callbacks_.input_removed);
      inputPortList = currInputPortList;

      auto currOutputPortList = get_port_list(OUTPUT);
      compare_port_lists_and_notify_clients(
          outputPortList, currOutputPortList, callbacks_.output_added, callbacks_.output_removed);
      outputPortList = currOutputPortList;
    }
  }

  void compare_port_lists_and_notify_clients(
      const PortList& prevList,
      const PortList& currList,
      const CallbackFunc& portAddedFunc,
      const CallbackFunc& portRemovedFunc)
  {
    if (portAddedFunc)
    {
      for (const auto& portName : currList)
      {
        auto iter = std::find(prevList.begin(), prevList.end(), portName);
        if (iter == prevList.end())
          portAddedFunc(0, portName);
      }
    }
    if (portRemovedFunc)
    {
      for (const auto portName : prevList)
      {
        auto iter = std::find(currList.begin(), currList.end(), portName);
        if (iter == currList.end())
          portRemovedFunc(0, portName);
      }
    }
  }

  bool wait_for_watch_thread_shutdown_signal(unsigned int timeoutMs)
  {
    using namespace std::chrono_literals;
    std::unique_lock<std::mutex> lock(watchThreadMutex);
    return watchThreadCV.wait_for(lock, timeoutMs * 1ms, [this]() { return watchThreadShutdown; });
  }

  void signal_watch_thread_shutdown()
  {
    std::lock_guard lock(watchThreadMutex);
    watchThreadShutdown = true;
  }

public:
  PortList get_port_list(bool input) const
  {
    // true Get input, false get output
    PortList portList;
    unsigned int nDevices = input ? midiInGetNumDevs() : midiOutGetNumDevs();
    for (unsigned int ix = 0; ix < nDevices; ++ix)
    {
      std::string portName;
      if (input)
      {
        MIDIINCAPS deviceCaps;
        midiInGetDevCaps(ix, &deviceCaps, sizeof(MIDIINCAPS));
        portName = ConvertToUTF8(deviceCaps.szPname);
      }
      else
      {
        MIDIOUTCAPS deviceCaps;
        midiOutGetDevCaps(ix, &deviceCaps, sizeof(MIDIOUTCAPS));
        portName = ConvertToUTF8(deviceCaps.szPname);
      }
      portList.push_back(portName);
    }
    return portList;
  }
};

class midi_in_winmm final : public midi_in_default<midi_in_winmm>
{
public:
  static const constexpr auto backend = "WinMM";
  midi_in_winmm(std::string_view, unsigned int queueSizeLimit)
      : midi_in_default{&data, queueSizeLimit}
  {
    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plugin something later.
    unsigned int nDevices = get_port_count();
    if (nDevices == 0)
    {
      warning("MidiInWinMM::initialize: no MIDI input devices currently available.");
    }

    if (!InitializeCriticalSectionAndSpinCount(&(data._mutex), 0x00000400))
    {
      warning("MidiInWinMM::initialize: InitializeCriticalSectionAndSpinCount failed.");
    }
  }

  ~midi_in_winmm() override
  {
    // Close a connection if it exists.
    midi_in_winmm::close_port();

    DeleteCriticalSection(&(data._mutex));
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::WINDOWS_MM;
  }

  void open_port(unsigned int portNumber, std::string_view) override
  {
    if (connected_)
    {
      warning("MidiInWinMM::openPort: a valid connection already exists!");
      return;
    }

    unsigned int nDevices = midiInGetNumDevs();
    if (nDevices == 0)
    {
      error<no_devices_found_error>("MidiInWinMM::openPort: no MIDI input sources found!");
      return;
    }

    if (portNumber >= nDevices)
    {
      std::ostringstream ost;
      ost << "MidiInWinMM::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
      error<invalid_parameter_error>(ost.str());
      return;
    }

    MMRESULT result = midiInOpen(
        &data.inHandle, portNumber, (DWORD_PTR)&midiInputCallback, (DWORD_PTR)&inputData_,
        CALLBACK_FUNCTION);
    if (result != MMSYSERR_NOERROR)
    {
      error<driver_error>("MidiInWinMM::openPort: error creating Windows MM MIDI input port.");
      return;
    }

    // Allocate and init the sysex buffers.
    for (int i = 0; i < RT_SYSEX_BUFFER_COUNT; ++i)
    {
      data.sysexBuffer[i] = (MIDIHDR*)new char[sizeof(MIDIHDR)];
      data.sysexBuffer[i]->lpData = new char[RT_SYSEX_BUFFER_SIZE];
      data.sysexBuffer[i]->dwBufferLength = RT_SYSEX_BUFFER_SIZE;
      data.sysexBuffer[i]->dwUser = i; // We use the dwUser parameter as buffer indicator
      data.sysexBuffer[i]->dwFlags = 0;

      result = midiInPrepareHeader(data.inHandle, data.sysexBuffer[i], sizeof(MIDIHDR));
      if (result != MMSYSERR_NOERROR)
      {
        midiInClose(data.inHandle);
        data.inHandle = nullptr;
        error<driver_error>(
            "MidiInWinMM::openPort: error starting Windows MM MIDI input port "
            "(PrepareHeader).");
        return;
      }

      // Register the buffer.
      result = midiInAddBuffer(data.inHandle, data.sysexBuffer[i], sizeof(MIDIHDR));
      if (result != MMSYSERR_NOERROR)
      {
        midiInClose(data.inHandle);
        data.inHandle = nullptr;
        error<driver_error>(
            "MidiInWinMM::openPort: error starting Windows MM MIDI input port "
            "(AddBuffer).");
        return;
      }
    }

    result = midiInStart(data.inHandle);
    if (result != MMSYSERR_NOERROR)
    {
      midiInClose(data.inHandle);
      data.inHandle = nullptr;
      error<driver_error>("MidiInWinMM::openPort: error starting Windows MM MIDI input port.");
      return;
    }

    connected_ = true;
  }

  void close_port() override
  {
    if (connected_)
    {
      EnterCriticalSection(&(data._mutex));
      midiInReset(data.inHandle);
      midiInStop(data.inHandle);

      for (int i = 0; i < RT_SYSEX_BUFFER_COUNT; ++i)
      {
        int result = midiInUnprepareHeader(data.inHandle, data.sysexBuffer[i], sizeof(MIDIHDR));
        delete[] data.sysexBuffer[i]->lpData;
        delete[] data.sysexBuffer[i];
        if (result != MMSYSERR_NOERROR)
        {
          midiInClose(data.inHandle);
          data.inHandle = nullptr;
          error<driver_error>(
              "MidiInWinMM::openPort: error closing Windows MM MIDI input "
              "port (midiInUnprepareHeader).");
          return;
        }
      }

      midiInClose(data.inHandle);
      data.inHandle = 0;
      connected_ = false;
      LeaveCriticalSection(&(data._mutex));
    }
  }

  unsigned int get_port_count() override
  {
    return midiInGetNumDevs();
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    std::string stringName;
    unsigned int nDevices = midiInGetNumDevs();
    if (portNumber >= nDevices)
    {
      std::ostringstream ost;
      ost << "MidiInWinMM::getPortName: the 'portNumber' argument (" << portNumber
          << ") is invalid.";
      warning(ost.str());
      return stringName;
    }

    MIDIINCAPS deviceCaps;
    midiInGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIINCAPS));
    stringName = ConvertToUTF8(deviceCaps.szPname);

    // Next lines added to add the portNumber to the name so that
    // the device's names are sure to be listed with individual names
    // even when they have the same brand name
#ifndef LIBREMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES
    int x = 1;
    for (unsigned int i = 0; i < portNumber; i++)
    {
      MIDIINCAPS deviceCaps2;
      midiInGetDevCaps(i, &deviceCaps2, sizeof(MIDIINCAPS));
      auto stringName2 = ConvertToUTF8(deviceCaps2.szPname);
      if (stringName == stringName2)
      {
        x++;
      }
    }
    std::ostringstream os;
    os << " ";
    os << x;
    stringName += os.str();
    x = 1;
#endif

    return stringName;
  }

private:
  static void CALLBACK midiInputCallback(
      HMIDIIN /*hmin*/,
      UINT inputStatus,
      DWORD_PTR instancePtr,
      DWORD_PTR midiMessage,
      DWORD timestamp)
  {
    if (inputStatus != MIM_DATA && inputStatus != MIM_LONGDATA && inputStatus != MIM_LONGERROR)
      return;

    // midi_in_api::RtMidiInData *data = static_cast<midi_in_api::RtMidiInData *>
    // (instancePtr);
    midi_in_api::in_data& data = *(midi_in_api::in_data*)instancePtr;
    WinMidiData& apiData = *static_cast<WinMidiData*>(data.apiData);

    // Calculate time stamp.
    if (data.firstMessage == true)
    {
      apiData.message.timestamp = 0.0;
      data.firstMessage = false;
    }
    else
      apiData.message.timestamp = (double)(timestamp - apiData.lastTime) * 0.001;

    if (inputStatus == MIM_DATA)
    { // Channel or system message

      // Make sure the first byte is a status byte.
      unsigned char status = (unsigned char)(midiMessage & 0x000000FF);
      if (!(status & 0x80))
        return;

      // Determine the number of bytes in the MIDI message.
      unsigned short nBytes = 1;
      if (status < 0xC0)
        nBytes = 3;
      else if (status < 0xE0)
        nBytes = 2;
      else if (status < 0xF0)
        nBytes = 3;
      else if (status == 0xF1)
      {
        if (data.ignoreFlags & 0x02)
          return;
        else
          nBytes = 2;
      }
      else if (status == 0xF2)
        nBytes = 3;
      else if (status == 0xF3)
        nBytes = 2;
      else if (status == 0xF8 && (data.ignoreFlags & 0x02))
      {
        // A MIDI timing tick message and we're ignoring it.
        return;
      }
      else if (status == 0xFE && (data.ignoreFlags & 0x04))
      {
        // A MIDI active sensing message and we're ignoring it.
        return;
      }

      // Copy bytes to our MIDI message.
      unsigned char* ptr = (unsigned char*)&midiMessage;
      apiData.message.bytes.resize(nBytes);
      for (int i = 0; i < nBytes; ++i)
        apiData.message.bytes[i] = ptr[i];
    }
    else
    { // Sysex message ( MIM_LONGDATA or MIM_LONGERROR )
      MIDIHDR* sysex = (MIDIHDR*)midiMessage;
      if (!(data.ignoreFlags & 0x01) && inputStatus != MIM_LONGERROR)
      {
        // Sysex message and we're not ignoring it
        for (int i = 0; i < (int)sysex->dwBytesRecorded; ++i)
          apiData.message.bytes.push_back(sysex->lpData[i]);
      }

      // The WinMM API requires that the sysex buffer be requeued after
      // input of each sysex message.  Even if we are ignoring sysex
      // messages, we still need to requeue the buffer in case the user
      // decides to not ignore sysex messages in the future.  However,
      // it seems that WinMM calls this function with an empty sysex
      // buffer when an application closes and in this case, we should
      // avoid requeueing it, else the computer suddenly reboots after
      // one or two minutes.
      if (apiData.sysexBuffer[sysex->dwUser]->dwBytesRecorded > 0)
      {
        // if ( sysex->dwBytesRecorded > 0 ) {
        EnterCriticalSection(&(apiData._mutex));
        MMRESULT result = midiInAddBuffer(
            apiData.inHandle, apiData.sysexBuffer[sysex->dwUser], sizeof(MIDIHDR));
        LeaveCriticalSection(&(apiData._mutex));
        if (result != MMSYSERR_NOERROR)
          std::cerr << "\nRtMidiIn::midiInputCallback: error sending sysex to "
                       "Midi device!!\n\n";

        if (data.ignoreFlags & 0x01)
          return;
      }
      else
        return;
    }

    // Save the time of the last non-filtered message
    apiData.lastTime = timestamp;

    data.on_message_received(std::move(apiData.message));
  }

  WinMidiData data;
};

class midi_out_winmm final : public midi_out_default<midi_out_winmm>
{
public:
  static const constexpr auto backend = "WinMM";
  midi_out_winmm(std::string_view)
  {
    // We'll issue a warning here if no devices are available but not
    // throw an error since the user can plug something in later.
    unsigned int nDevices = get_port_count();
    if (nDevices == 0)
    {
      warning(
          "MidiOutWinMM::initialize: no MIDI output devices currently "
          "available.");
    }
  }

  ~midi_out_winmm() override
  {
    // Close a connection if it exists.
    midi_out_winmm::close_port();
  }

  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::WINDOWS_MM;
  }

  void open_port(unsigned int portNumber, std::string_view) override
  {
    if (connected_)
    {
      warning("MidiOutWinMM::openPort: a valid connection already exists!");
      return;
    }

    unsigned int nDevices = midiOutGetNumDevs();
    if (nDevices < 1)
    {
      error<no_devices_found_error>("MidiOutWinMM::openPort: no MIDI output destinations found!");
      return;
    }

    if (portNumber >= nDevices)
    {
      std::ostringstream ost;
      ost << "MidiOutWinMM::openPort: the 'portNumber' argument (" << portNumber
          << ") is invalid.";
      error<invalid_parameter_error>(ost.str());
      return;
    }

    MMRESULT result = midiOutOpen(&data.outHandle, portNumber, NULL, NULL, CALLBACK_NULL);
    if (result != MMSYSERR_NOERROR)
    {
      error<driver_error>(
          "MidiOutWinMM::openPort: error creating Windows MM MIDI output "
          "port.");
      return;
    }

    connected_ = true;
  }

  void close_port() override
  {
    if (connected_)
    {
      midiOutReset(data.outHandle);
      midiOutClose(data.outHandle);
      data.outHandle = nullptr;
      connected_ = false;
    }
  }

  unsigned int get_port_count() override
  {
    return midiOutGetNumDevs();
  }

  std::string get_port_name(unsigned int portNumber) override
  {
    std::string stringName;
    unsigned int nDevices = midiOutGetNumDevs();
    if (portNumber >= nDevices)
    {
      std::ostringstream ost;
      ost << "MidiOutWinMM::getPortName: the 'portNumber' argument (" << portNumber
          << ") is invalid.";
      warning(ost.str());
      return stringName;
    }

    MIDIOUTCAPS deviceCaps;

    midiOutGetDevCaps(portNumber, &deviceCaps, sizeof(MIDIOUTCAPS));
    stringName = ConvertToUTF8(deviceCaps.szPname);

    // Next lines added to add the portNumber to the name so that
    // the device's names are sure to be listed with individual names
    // even when they have the same brand name
    std::ostringstream os;
#ifndef LIBREMIDI_DO_NOT_ENSURE_UNIQUE_PORTNAMES
    int x = 1;
    for (unsigned int i = 0; i < portNumber; i++)
    {
      MIDIOUTCAPS deviceCaps2;
      midiOutGetDevCaps(i, &deviceCaps2, sizeof(MIDIOUTCAPS));
      auto sn2 = ConvertToUTF8(deviceCaps2.szPname);
      if (stringName == sn2)
      {
        x++;
      }
    }
    os << " ";
    os << x;
    stringName += os.str();
    x = 1;
#endif

    return stringName;
  }

  void send_message(const unsigned char* message, size_t size) override
  {
    if (!connected_)
      return;

    unsigned int nBytes = static_cast<unsigned int>(size);
    if (nBytes == 0)
    {
      warning("MidiOutWinMM::sendMessage: message argument is empty!");
      return;
    }

    MMRESULT result;
    if (message[0] == 0xF0)
    { // Sysex message

      // Allocate buffer for sysex data.
      buffer.clear();
      buffer.resize(nBytes);

      // Copy data to buffer.
      for (unsigned int i = 0; i < nBytes; ++i)
        buffer[i] = message[i];

      // Create and prepare MIDIHDR structure.
      MIDIHDR sysex;
      sysex.lpData = (LPSTR)buffer.data();
      sysex.dwBufferLength = nBytes;
      sysex.dwFlags = 0;
      result = midiOutPrepareHeader(data.outHandle, &sysex, sizeof(MIDIHDR));
      if (result != MMSYSERR_NOERROR)
      {
        error<driver_error>("MidiOutWinMM::sendMessage: error preparing sysex header.");
        return;
      }

      // Send the message.
      result = midiOutLongMsg(data.outHandle, &sysex, sizeof(MIDIHDR));
      if (result != MMSYSERR_NOERROR)
      {
        error<driver_error>("MidiOutWinMM::sendMessage: error sending sysex message.");
        return;
      }

      // Unprepare the buffer and MIDIHDR.
      while (MIDIERR_STILLPLAYING
             == midiOutUnprepareHeader(data.outHandle, &sysex, sizeof(MIDIHDR)))
        Sleep(1);
    }
    else
    { // Channel or system message.

      // Make sure the message size isn't too big.
      if (nBytes > 3)
      {
        warning(
            "MidiOutWinMM::sendMessage: message size is greater than 3 bytes "
            "(and not sysex)!");
        return;
      }

      // Pack MIDI bytes into double word.
      DWORD packet;
      unsigned char* ptr = (unsigned char*)&packet;
      for (unsigned int i = 0; i < nBytes; ++i)
      {
        *ptr = message[i];
        ++ptr;
      }

      // Send the message immediately.
      result = midiOutShortMsg(data.outHandle, packet);
      if (result != MMSYSERR_NOERROR)
      {
        error<driver_error>("MidiOutWinMM::sendMessage: error sending MIDI message.");
      }
    }
  }

private:
  WinMidiData data;
  std::vector<char> buffer;
};

struct winmm_backend
{
  using midi_in = midi_in_winmm;
  using midi_out = midi_out_winmm;
  using midi_observer = observer_winmm;
  static const constexpr auto API = libremidi::API::WINDOWS_MM;
};
}
