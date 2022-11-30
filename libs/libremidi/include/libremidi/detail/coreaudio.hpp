#pragma once
#include <CoreMIDI/CoreMIDI.h>
#include <CoreServices/CoreServices.h>
#include <cmath>
#include <libremidi/detail/midi_api.hpp>
#include <libremidi/libremidi.hpp>

#if TARGET_OS_IPHONE
#include <CoreAudio/CoreAudioTypes.h>
#include <mach/mach_time.h>
#define AudioGetCurrentHostTime mach_absolute_time
#else
#include <CoreAudio/HostTime.h>
#endif

namespace libremidi
{
#if TARGET_OS_IPHONE
namespace {
inline uint64_t AudioConvertHostTimeToNanos(uint64_t hostTime)
{
    static const struct mach_timebase_info timebase = [] {
        struct mach_timebase_info theTimeBaseInfo;
        mach_timebase_info(&theTimeBaseInfo);
        return theTimeBaseInfo;
    }();
    const auto numer = timebase.numer;
    const auto denom = timebase.denom;

    __uint128_t res = hostTime;
    if(numer != denom)
    {
      res *= numer;
      res /= denom;
    }
    return static_cast<uint64_t>(res);
}
}
#endif
// This function was submitted by Douglas Casey Tucker and apparently
// derived largely from PortMidi.
inline CFStringRef EndpointName(MIDIEndpointRef endpoint, bool isExternal)
{
  CFMutableStringRef result = CFStringCreateMutable(nullptr, 0);
  CFStringRef str;

  // Begin with the endpoint's name.
  str = nullptr;
  MIDIObjectGetStringProperty(endpoint, kMIDIPropertyName, &str);
  if (str != nullptr)
  {
    CFStringAppend(result, str);
    CFRelease(str);
  }

  MIDIEntityRef entity = 0;
  MIDIEndpointGetEntity(endpoint, &entity);
  if (entity == 0)
    // probably virtual
    return result;

  if (CFStringGetLength(result) == 0)
  {
    // endpoint name has zero length -- try the entity
    str = nullptr;
    MIDIObjectGetStringProperty(entity, kMIDIPropertyName, &str);
    if (str != nullptr)
    {
      CFStringAppend(result, str);
      CFRelease(str);
    }
  }
  // now consider the device's name
  MIDIDeviceRef device = 0;
  MIDIEntityGetDevice(entity, &device);
  if (device == 0)
    return result;

  str = nullptr;
  MIDIObjectGetStringProperty(device, kMIDIPropertyName, &str);
  if (CFStringGetLength(result) == 0)
  {
    CFRelease(result);
    return str;
  }
  if (str != nullptr)
  {
    // if an external device has only one entity, throw away
    // the endpoint name and just use the device name
    if (isExternal && MIDIDeviceGetNumberOfEntities(device) < 2)
    {
      CFRelease(result);
      return str;
    }
    else
    {
      if (CFStringGetLength(str) == 0)
      {
        CFRelease(str);
        return result;
      }
      // does the entity name already start with the device name?
      // (some drivers do this though they shouldn't)
      // if so, do not prepend
      if (CFStringCompareWithOptions(
              result, /* endpoint name */
              str /* device name */, CFRangeMake(0, CFStringGetLength(str)), 0)
          != kCFCompareEqualTo)
      {
        // prepend the device name to the entity name
        if (CFStringGetLength(result) > 0)
          CFStringInsert(result, 0, CFSTR(" "));
        CFStringInsert(result, 0, str);
      }
      CFRelease(str);
    }
  }
  return result;
}

// This function was submitted by Douglas Casey Tucker and apparently
// derived largely from PortMidi.
inline CFStringRef ConnectedEndpointName(MIDIEndpointRef endpoint)
{
  CFMutableStringRef result = CFStringCreateMutable(nullptr, 0);
  CFStringRef str;
  OSStatus err;
  int i;

  // Does the endpoint have connections?
  CFDataRef connections = nullptr;
  int nConnected = 0;
  bool anyStrings = false;
  err = MIDIObjectGetDataProperty(endpoint, kMIDIPropertyConnectionUniqueID, &connections);
  if (connections != nullptr)
  {
    // It has connections, follow them
    // Concatenate the names of all connected devices
    nConnected = CFDataGetLength(connections) / sizeof(MIDIUniqueID);
    if (nConnected)
    {
      const SInt32* pid = (const SInt32*)(CFDataGetBytePtr(connections));
      for (i = 0; i < nConnected; ++i, ++pid)
      {
        MIDIUniqueID id = CFSwapInt32BigToHost(*pid);
        MIDIObjectRef connObject;
        MIDIObjectType connObjectType;
        err = MIDIObjectFindByUniqueID(id, &connObject, &connObjectType);
        if (err == noErr)
        {
          if (connObjectType == kMIDIObjectType_ExternalSource
              || connObjectType == kMIDIObjectType_ExternalDestination)
          {
            // Connected to an external device's endpoint (10.3 and later).
            str = EndpointName((MIDIEndpointRef)(connObject), true);
          }
          else
          {
            // Connected to an external device (10.2) (or something else,
            // catch-
            str = nullptr;
            MIDIObjectGetStringProperty(connObject, kMIDIPropertyName, &str);
          }
          if (str != nullptr)
          {
            if (anyStrings)
              CFStringAppend(result, CFSTR(", "));
            else
              anyStrings = true;
            CFStringAppend(result, str);
            CFRelease(str);
          }
        }
      }
    }
    CFRelease(connections);
  }
  if (anyStrings)
    return result;

  CFRelease(result);

  // Here, either the endpoint had no connections, or we failed to obtain names
  return EndpointName(endpoint, false);
}

// A structure to hold variables related to the CoreMIDI API
// implementation.
struct coremidi_data
{
  MIDIClientRef client{};
  MIDIPortRef port{};
  MIDIEndpointRef endpoint{};
  MIDIEndpointRef destinationId{};
  unsigned long long lastTime{};
  MIDISysexSendRequest sysexreq{};
};

class observer_core final : public observer_api
{
public:
  observer_core(observer::callbacks&& c) : observer_api{std::move(c)}
  {
  }

  ~observer_core()
  {
  }
};

class midi_in_core final : public midi_in_api
{
public:
  midi_in_core(std::string_view clientName, unsigned int queueSizeLimit)
      : midi_in_api(&data, queueSizeLimit)
  {
    // Set up our client.
    MIDIClientRef client{};
    CFStringRef name
        = CFStringCreateWithCString(nullptr, clientName.data(), kCFStringEncodingASCII);
    OSStatus result = MIDIClientCreate(name, nullptr, nullptr, &client);
    if (result != noErr)
    {
      std::ostringstream ost;
      ost << "MidiInCore::initialize: error creating OS-X MIDI client object (" << result << ").";
      error<driver_error>(ost.str());
      return;
    }

    // Save our api-specific connection information.
    data.client = client;
    data.endpoint = 0;
    CFRelease(name);
  }

  ~midi_in_core() override
  {
    // Close a connection if it exists.
    midi_in_core::close_port();

    // Cleanup.
    MIDIClientDispose(data.client);
    if (data.endpoint)
      MIDIEndpointDispose(data.endpoint);
  }
  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::MACOSX_CORE;
  }
  void open_port(unsigned int portNumber, std::string_view portName) override
  {
    if (connected_)
    {
      warning("MidiInCore::openPort: a valid connection already exists!");
      return;
    }

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    unsigned int nSrc = MIDIGetNumberOfSources();
    if (nSrc < 1)
    {
      error<no_devices_found_error>("MidiInCore::openPort: no MIDI input sources found!");
      return;
    }

    if (portNumber >= nSrc)
    {
      std::ostringstream ost;
      ost << "MidiInCore::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
      error<invalid_parameter_error>(ost.str());
      return;
    }

    MIDIPortRef port;
    CFStringRef portNameRef
        = CFStringCreateWithCString(nullptr, portName.data(), kCFStringEncodingASCII);
    OSStatus result = MIDIInputPortCreate(
        data.client, portNameRef, midiInputCallback, (void*)&inputData_, &port);
    CFRelease(portNameRef);

    if (result != noErr)
    {
      MIDIClientDispose(data.client);
      error<driver_error>("MidiInCore::openPort: error creating OS-X MIDI input port.");
      return;
    }

    // Get the desired input source identifier.
    MIDIEndpointRef endpoint = MIDIGetSource(portNumber);
    if (endpoint == 0)
    {
      MIDIPortDispose(port);
      MIDIClientDispose(data.client);
      error<driver_error>("MidiInCore::openPort: error getting MIDI input source reference.");
      return;
    }

    // Make the connection.
    result = MIDIPortConnectSource(port, endpoint, nullptr);
    if (result != noErr)
    {
      MIDIPortDispose(port);
      MIDIClientDispose(data.client);
      error<driver_error>("MidiInCore::openPort: error connecting OS-X MIDI input port.");
      return;
    }

    // Save our api-specific port information.
    data.port = port;

    connected_ = true;
  }
  void open_virtual_port(std::string_view portName) override
  {
    // Create a virtual MIDI input destination.
    MIDIEndpointRef endpoint;
    CFStringRef portNameRef
        = CFStringCreateWithCString(nullptr, portName.data(), kCFStringEncodingASCII);
    OSStatus result = MIDIDestinationCreate(
        data.client, portNameRef, midiInputCallback, (void*)&inputData_, &endpoint);
    CFRelease(portNameRef);

    if (result != noErr)
    {
      error<driver_error>(
          "MidiInCore::openVirtualPort: error creating virtual OS-X MIDI "
          "destination.");
      return;
    }

    // Save our api-specific connection information.
    data.endpoint = endpoint;
  }
  void close_port() override
  {
    if (data.endpoint)
    {
      MIDIEndpointDispose(data.endpoint);
      data.endpoint = 0;
    }

    if (data.port)
    {
      MIDIPortDispose(data.port);
      data.port = 0;
    }

    connected_ = false;
  }
  void set_client_name(std::string_view clientName) override
  {
    warning(
        "MidiInCore::setClientName: this function is not implemented for the "
        "MACOSX_CORE API!");
  }
  void set_port_name(std::string_view portName) override
  {
    warning(
        "MidiInCore::setPortName: this function is not implemented for the "
        "MACOSX_CORE API!");
  }
  unsigned int get_port_count() override
  {
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    return MIDIGetNumberOfSources();
  }
  std::string get_port_name(unsigned int portNumber) override
  {
    CFStringRef nameRef;
    MIDIEndpointRef portRef;
    char name[128];

    std::string stringName;
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    if (portNumber >= MIDIGetNumberOfSources())
    {
      std::ostringstream ost;
      ost << "MidiInCore::getPortName: the 'portNumber' argument (" << portNumber
          << ") is invalid.";
      warning(ost.str());
      return stringName;
    }

    portRef = MIDIGetSource(portNumber);
    nameRef = ConnectedEndpointName(portRef);
    CFStringGetCString(nameRef, name, sizeof(name), kCFStringEncodingUTF8);
    CFRelease(nameRef);

    return stringName = name;
  }

private:
  static void midiInputCallback(const MIDIPacketList* list, void* procRef, void* /*srcRef*/)
  {
    auto& data = *static_cast<midi_in_api::in_data*>(procRef);
    auto apiData = static_cast<coremidi_data*>(data.apiData);

    unsigned char status{};
    unsigned short nBytes{}, iByte{}, size{};
    unsigned long long time{};

    bool& continueSysex = data.continueSysex;
    message& msg = data.message;

    const MIDIPacket* packet = &list->packet[0];
    for (unsigned int i = 0; i < list->numPackets; ++i)
    {

      // My interpretation of the CoreMIDI documentation: all message
      // types, except sysex, are complete within a packet and there may
      // be several of them in a single packet.  Sysex messages can be
      // broken across multiple packets and PacketLists but are bundled
      // alone within each packet (these packets do not contain other
      // message types).  If sysex messages are split across multiple
      // MIDIPacketLists, they must be handled by multiple calls to this
      // function.

      nBytes = packet->length;
      if (nBytes == 0)
      {
        packet = MIDIPacketNext(packet);
        continue;
      }

      // Calculate time stamp.
      if (data.firstMessage)
      {
        msg.timestamp = 0.0;
        data.firstMessage = false;
      }
      else
      {
        time = packet->timeStamp;
        if (time == 0)
        { // this happens when receiving asynchronous sysex messages
          time = AudioGetCurrentHostTime();
        }
        time -= apiData->lastTime;
        time = AudioConvertHostTimeToNanos(time);
        if (!continueSysex)
          msg.timestamp = time * 0.000000001;
      }

      // Track whether any non-filtered messages were found in this
      // packet for timestamp calculation
      bool foundNonFiltered = false;

      iByte = 0;
      if (continueSysex)
      {
        // We have a continuing, segmented sysex message.
        if (!(data.ignoreFlags & 0x01))
        {
          // If we're not ignoring sysex messages, copy the entire packet.
          for (unsigned int j = 0; j < nBytes; ++j)
            msg.bytes.push_back(packet->data[j]);
        }
        continueSysex = packet->data[nBytes - 1] != 0xF7;

        if (!(data.ignoreFlags & 0x01) && !continueSysex)
        {
          // If not a continuing sysex message, invoke the user callback
          // function or queue the message.
          data.on_message_received(std::move(msg));
        }
      }
      else
      {
        while (iByte < nBytes)
        {
          size = 0;
          // We are expecting that the next byte in the packet is a status
          // byte.
          status = packet->data[iByte];
          if (!(status & 0x80))
            break;
          // Determine the number of bytes in the MIDI message.
          if (status < 0xC0)
            size = 3;
          else if (status < 0xE0)
            size = 2;
          else if (status < 0xF0)
            size = 3;
          else if (status == 0xF0)
          {
            // A MIDI sysex
            if (data.ignoreFlags & 0x01)
            {
              size = 0;
              iByte = nBytes;
            }
            else
              size = nBytes - iByte;
            continueSysex = packet->data[nBytes - 1] != 0xF7;
          }
          else if (status == 0xF1)
          {
            // A MIDI time code message
            if (data.ignoreFlags & 0x02)
            {
              size = 0;
              iByte += 2;
            }
            else
              size = 2;
          }
          else if (status == 0xF2)
            size = 3;
          else if (status == 0xF3)
            size = 2;
          else if (status == 0xF8 && (data.ignoreFlags & 0x02))
          {
            // A MIDI timing tick message and we're ignoring it.
            size = 0;
            iByte += 1;
          }
          else if (status == 0xFE && (data.ignoreFlags & 0x04))
          {
            // A MIDI active sensing message and we're ignoring it.
            size = 0;
            iByte += 1;
          }
          else
            size = 1;

          // Copy the MIDI data to our vector.
          if (size)
          {
            foundNonFiltered = true;
            msg.bytes.assign(&packet->data[iByte], &packet->data[iByte + size]);
            if (!continueSysex)
            {
              // If not a continuing sysex message, invoke the user callback
              // function or queue the message.
              data.on_message_received(std::move(msg));
            }
            iByte += size;
          }
        }
      }

      // Save the time of the last non-filtered message
      if (foundNonFiltered)
      {
        apiData->lastTime = packet->timeStamp;
        if (apiData->lastTime == 0)
        { // this happens when receiving asynchronous sysex messages
          apiData->lastTime = AudioGetCurrentHostTime();
        }
      }

      packet = MIDIPacketNext(packet);
    }
  }
  coremidi_data data;
};

class midi_out_core final : public midi_out_api
{
public:
  midi_out_core(std::string_view clientName)
  {
    // Set up our client.
    MIDIClientRef client;
    CFStringRef name
        = CFStringCreateWithCString(nullptr, clientName.data(), kCFStringEncodingASCII);
    OSStatus result = MIDIClientCreate(name, nullptr, nullptr, &client);
    if (result != noErr)
    {
      std::ostringstream ost;
      ost << "MidiInCore::initialize: error creating OS-X MIDI client object (" << result << ").";
      error<driver_error>(ost.str());
      return;
    }

    // Save our api-specific connection information.
    data.client = client;
    data.endpoint = 0;
    CFRelease(name);
  }
  ~midi_out_core()
  {
    // Close a connection if it exists.
    midi_out_core::close_port();

    // Cleanup.
    MIDIClientDispose(data.client);
    if (data.endpoint)
      MIDIEndpointDispose(data.endpoint);
  }
  libremidi::API get_current_api() const noexcept override
  {
    return libremidi::API::MACOSX_CORE;
  }
  void open_port(unsigned int portNumber, std::string_view portName) override
  {
    if (connected_)
    {
      warning("MidiOutCore::openPort: a valid connection already exists!");
      return;
    }

    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    unsigned int nDest = MIDIGetNumberOfDestinations();
    if (nDest < 1)
    {
      error<no_devices_found_error>("MidiOutCore::openPort: no MIDI output destinations found!");
      return;
    }

    if (portNumber >= nDest)
    {
      std::ostringstream ost;
      ost << "MidiOutCore::openPort: the 'portNumber' argument (" << portNumber << ") is invalid.";
      error<invalid_parameter_error>(ost.str());
      return;
    }

    MIDIPortRef port;
    CFStringRef portNameRef
        = CFStringCreateWithCString(nullptr, portName.data(), kCFStringEncodingASCII);
    OSStatus result = MIDIOutputPortCreate(data.client, portNameRef, &port);
    CFRelease(portNameRef);
    if (result != noErr)
    {
      MIDIClientDispose(data.client);
      error<driver_error>("MidiOutCore::openPort: error creating OS-X MIDI output port.");
      return;
    }

    // Get the desired output port identifier.
    MIDIEndpointRef destination = MIDIGetDestination(portNumber);
    if (destination == 0)
    {
      MIDIPortDispose(port);
      MIDIClientDispose(data.client);
      error<driver_error>(
          "MidiOutCore::openPort: error getting MIDI output destination "
          "reference.");
      return;
    }

    // Save our api-specific connection information.
    data.port = port;
    data.destinationId = destination;
    connected_ = true;
  }
  void open_virtual_port(std::string_view portName) override
  {
    if (data.endpoint)
    {
      warning(
          "MidiOutCore::openVirtualPort: a virtual output port already "
          "exists!");
      return;
    }

    // Create a virtual MIDI output source.
    MIDIEndpointRef endpoint;
    CFStringRef portNameRef
        = CFStringCreateWithCString(nullptr, portName.data(), kCFStringEncodingASCII);
    OSStatus result = MIDISourceCreate(data.client, portNameRef, &endpoint);
    CFRelease(portNameRef);

    if (result != noErr)
    {
      error<driver_error>("MidiOutCore::initialize: error creating OS-X virtual MIDI source.");
      return;
    }

    // Save our api-specific connection information.
    data.endpoint = endpoint;
  }
  void close_port() override
  {
    if (data.endpoint)
    {
      MIDIEndpointDispose(data.endpoint);
      data.endpoint = 0;
    }

    if (data.port)
    {
      MIDIPortDispose(data.port);
      data.port = 0;
    }

    connected_ = false;
  }
  void set_client_name(std::string_view clientName) override
  {
    warning(
        "MidiOutCore::set_client_name: this function is not implemented for "
        "the "
        "MACOSX_CORE API!");
  }
  void set_port_name(std::string_view portName) override
  {
    warning(
        "MidiOutCore::set_port_name: this function is not implemented for the "
        "MACOSX_CORE API!");
  }
  unsigned int get_port_count() override
  {
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    return MIDIGetNumberOfDestinations();
  }
  std::string get_port_name(unsigned int portNumber) override
  {
    CFStringRef nameRef;
    MIDIEndpointRef portRef;
    char name[128];

    std::string stringName;
    CFRunLoopRunInMode(kCFRunLoopDefaultMode, 0, false);
    if (portNumber >= MIDIGetNumberOfDestinations())
    {
      std::ostringstream ost;
      ost << "MidiOutCore::getPortName: the 'portNumber' argument (" << portNumber
          << ") is invalid.";
      warning(ost.str());
      return stringName;
    }

    portRef = MIDIGetDestination(portNumber);
    nameRef = ConnectedEndpointName(portRef);
    CFStringGetCString(nameRef, name, sizeof(name), kCFStringEncodingUTF8);
    CFRelease(nameRef);

    return stringName = name;
  }
  void send_message(const unsigned char* message, size_t size) override
  {
    // We use the MIDISendSysex() function to asynchronously send sysex
    // messages.  Otherwise, we use a single CoreMidi MIDIPacket.
    unsigned int nBytes = static_cast<unsigned int>(size);
    if (nBytes == 0)
    {
      warning("MidiOutCore::sendMessage: no data in message argument!");
      return;
    }

    MIDITimeStamp timestamp = AudioGetCurrentHostTime();
    OSStatus result;

    if (message[0] != 0xF0 && nBytes > 3)
    {
      warning(
          "MidiOutCore::sendMessage: message format problem ... not sysex but "
          "> 3 bytes?");
      return;
    }

    ByteCount listSize = nBytes + (sizeof(MIDIPacketList));
    Byte* buffer = (Byte*)alloca(listSize);
    MIDIPacketList* packetList = (MIDIPacketList*)buffer;
    MIDIPacket* packet = MIDIPacketListInit(packetList);

    ByteCount remainingBytes = nBytes;
    while (remainingBytes && packet)
    {
      ByteCount bytesForPacket = remainingBytes > 65535
                                     ? 65535
                                     : remainingBytes; // 65535 = maximum size of a MIDIPacket
      const Byte* dataStartPtr = (const Byte*)&message[nBytes - remainingBytes];

      packet = MIDIPacketListAdd(
          packetList, listSize, packet, timestamp, bytesForPacket, dataStartPtr);
      remainingBytes -= bytesForPacket;
    }

    if (!packet)
    {
      error<driver_error>("MidiOutCore::sendMessage: could not allocate packet list");
      return;
    }

    // Send to any destinations that may have connected to us.
    if (data.endpoint)
    {
      result = MIDIReceived(data.endpoint, packetList);
      if (result != noErr)
      {
        warning(
            "MidiOutCore::sendMessage: error sending MIDI to virtual "
            "destinations.");
      }
    }

    // And send to an explicit destination port if we're connected.
    if (connected_)
    {
      result = MIDISend(data.port, data.destinationId, packetList);
      if (result != noErr)
      {
        warning("MidiOutCore::sendMessage: error sending MIDI message to port.");
      }
    }
  }

private:
  coremidi_data data;
};

struct core_backend
{
  using midi_in = midi_in_core;
  using midi_out = midi_out_core;
  using midi_observer = observer_core;
  static const constexpr auto API = libremidi::API::MACOSX_CORE;
};
}
#if TARGET_OS_IPHONE
#undef AudioGetCurrentHostTime
#endif
