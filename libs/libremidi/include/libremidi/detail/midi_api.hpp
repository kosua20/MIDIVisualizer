#pragma once
#include <iostream>
#include <libremidi/libremidi.hpp>
#include <libremidi/detail/midi_queue.hpp>
#include <string_view>

namespace libremidi
{
class observer_api
{
public:
  observer_api(observer::callbacks c) : callbacks_{std::move(c)}
  {
  }

  virtual ~observer_api() = default;

protected:
  observer::callbacks callbacks_;
};

class midi_api
{
public:
  midi_api() = default;
  virtual ~midi_api() = default;
  midi_api(const midi_api&) = delete;
  midi_api(midi_api&&) = delete;
  midi_api& operator=(const midi_api&) = delete;
  midi_api& operator=(midi_api&&) = delete;

  virtual libremidi::API get_current_api() const noexcept = 0;
  virtual void open_port(unsigned int portNumber, std::string_view portName) = 0;
  virtual void open_virtual_port(std::string_view) = 0;
  virtual void close_port() = 0;
  virtual void set_client_name(std::string_view) = 0;
  virtual void set_port_name(std::string_view) = 0;

  virtual unsigned int get_port_count() = 0;
  virtual std::string get_port_name(unsigned int portNumber) = 0;

  bool is_port_open() const noexcept
  {
    return bool(connected_);
  }

  void set_error_callback(midi_error_callback errorCallback) noexcept
  {
    errorCallback_ = std::move(errorCallback);
  }

  //! Error reporting function for RtMidi classes. Throws.
  template <typename Error_T>
  void error(std::string_view errorString) const
  {
    if (errorCallback_)
    {
      if (firstErrorOccurred_)
      {
        return;
      }

      firstErrorOccurred_ = true;
      errorCallback_(Error_T::code, errorString);
      firstErrorOccurred_ = false;
    }
    else
    {
      std::cerr << '\n' << errorString << "\n\n";
      throw Error_T{errorString.data()};
    }
  }

  //! Warning reporting function for RtMidi classes.
  void warning(std::string_view errorString) const
  {
    if (errorCallback_)
    {
      if (firstErrorOccurred_)
      {
        return;
      }

      firstErrorOccurred_ = true;
      errorCallback_(midi_error::WARNING, errorString);
      firstErrorOccurred_ = false;
      return;
    }

    std::cerr << '\n' << errorString << "\n\n";
  }

protected:
  midi_error_callback errorCallback_{};
  bool connected_{};
  mutable bool firstErrorOccurred_{};
};

class midi_in_api : public midi_api
{
public:
  explicit midi_in_api(void* data, unsigned int queueSizeLimit)
  {
    inputData_.apiData = data;
    // Allocate the MIDI queue.
    inputData_.queue.ringSize = queueSizeLimit;
    if (inputData_.queue.ringSize > 0)
    {
      inputData_.queue.ring = std::make_unique<libremidi::message[]>(inputData_.queue.ringSize);
    }
  }
  ~midi_in_api() override = default;

  midi_in_api(const midi_in_api&) = delete;
  midi_in_api(midi_in_api&&) = delete;
  midi_in_api& operator=(const midi_in_api&) = delete;
  midi_in_api& operator=(midi_in_api&&) = delete;

  virtual void ignore_types(bool midiSysex, bool midiTime, bool midiSense)
  {
    inputData_.ignoreFlags = 0;
    if (midiSysex)
    {
      inputData_.ignoreFlags = 0x01;
    }
    if (midiTime)
    {
      inputData_.ignoreFlags |= 0x02;
    }
    if (midiSense)
    {
      inputData_.ignoreFlags |= 0x04;
    }
  }

  void set_callback(midi_in::message_callback callback)
  {
    inputData_.userCallback = std::move(callback);
  }

  void cancel_callback()
  {
    inputData_.userCallback = midi_in::message_callback{};
  }

  message get_message()
  {
    if (inputData_.userCallback)
    {
      warning(
          "RtMidiIn::getNextMessage: a user callback is currently set for "
          "this port.");
      return {};
    }

    message m;
    if (inputData_.queue.pop(m))
    {
      return m;
    }
    return {};
  }

  bool get_message(message& m)
  {
    if (inputData_.userCallback)
    {
      warning(
          "RtMidiIn::getNextMessage: a user callback is currently set for "
          "this port.");
      return {};
    }

    return inputData_.queue.pop(m);
  }

  // The RtMidiInData structure is used to pass private class data to
  // the MIDI input handling function or thread.
  struct in_data
  {
    midi_queue queue{};
    libremidi::message message{};
    unsigned char ignoreFlags{7};
    bool doInput{false};
    bool firstMessage{true};
    void* apiData{};
    midi_in::message_callback userCallback{};
    bool continueSysex{false};

    void on_message_received(libremidi::message&& message)
    {
      if (userCallback)
      {
        userCallback(std::move(message));
      }
      else
      {
        // As long as we haven't reached our queue size limit, push the
        // message.
        if (!queue.push(std::move(message)))
          std::cerr << "\nmidi_in: message queue limit reached!!\n\n";
      }
      message.bytes.clear();
    }
  };

protected:
  in_data inputData_{};
};

class midi_out_api : public midi_api
{
public:
  virtual void send_message(const unsigned char* message, size_t size) = 0;

  void set_chunking_parameters(std::optional<chunking_parameters> parameters)
  {
    chunking = std::move(parameters);
  }

  std::optional<chunking_parameters> chunking;
};

template <typename T>
class midi_in_default : public midi_in_api
{
  using midi_in_api::midi_in_api;
  void open_virtual_port(std::string_view) override
  {
    using namespace std::literals;
    warning(T::backend + " in: open_virtual_port unsupported"s);
  }
  void set_client_name(std::string_view) override
  {
    using namespace std::literals;
    warning(T::backend + " in: set_client_name unsupported"s);
  }
  void set_port_name(std::string_view) override
  {
    using namespace std::literals;
    warning(T::backend + " in: set_port_name unsupported"s);
  }
};

template <typename T>
class midi_out_default : public midi_out_api
{
  using midi_out_api::midi_out_api;
  void open_virtual_port(std::string_view) override
  {
    using namespace std::literals;
    warning(T::backend + " out: open_virtual_port unsupported"s);
  }
  void set_client_name(std::string_view) override
  {
    using namespace std::literals;
    warning(T::backend + " out: set_client_name unsupported"s);
  }
  void set_port_name(std::string_view) override
  {
    using namespace std::literals;
    warning(T::backend + " out: set_port_name unsupported"s);
  }
};
}
