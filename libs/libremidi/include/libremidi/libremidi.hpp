#pragma once
/* This software is based on the RtMidi and ModernMidi libraries.

  RtMidi WWW site: http://music.mcgill.ca/~gary/libremidi/

  RtMidi: realtime MIDI i/o C++ classes
  Copyright (c) 2003-2017 Gary P. Scavone

  Permission is hereby granted, free of charge, to any person
  obtaining a copy of this software and associated documentation files
  (the "Software"), to deal in the Software without restriction,
  including without limitation the rights to use, copy, modify, merge,
  publish, distribute, sublicense, and/or sell copies of the Software,
  and to permit persons to whom the Software is furnished to do so,
  subject to the following conditions:

  The above copyright notice and this permission notice shall be
  included in all copies or substantial portions of the Software.

  Any person wishing to distribute modifications to the Software is
  asked to send the modifications to the original developer so that
  they can be incorporated into the canonical version.  This is,
  however, not a binding provision of this license.

  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND,
  EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF
  MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.
  IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR
  ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF
  CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION
  WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.

 ------------

  ModernMidi Copyright (c) 2015, Dimitri Diakopoulos All rights reserved.

  Redistribution and use in source and binary forms, with or without
  modification, are permitted provided that the following conditions are met:

  * Redistributions of source code must retain the above copyright notice, this
    list of conditions and the following disclaimer.

  * Redistributions in binary form must reproduce the above copyright notice,
    this list of conditions and the following disclaimer in the documentation
    and/or other materials provided with the distribution.

  THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
  AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
  IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE
 LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR
 CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF
 SUBSTITUTE GOODS OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS
 INTERRUPTION) HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN
 CONTRACT, STRICT LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE)
 ARISING IN ANY WAY OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE
 POSSIBILITY OF SUCH DAMAGE.
*/

#include <libremidi/message.hpp>
#include <libremidi/api.hpp>

#include <algorithm>
#include <cassert>
#include <chrono>
#include <functional>
#include <iostream>
#include <memory>
#include <optional>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

#if defined(__cpp_lib_span) && __cpp_lib_span >= 202002
#define LIBREMIDI_HAS_SPAN 1
#include <span>
#endif

#if defined(LIBREMIDI_EXPORTS)
#  if defined(_MSC_VER)
#    define LIBREMIDI_EXPORT __declspec(dllexport)
#  elif defined(__GNUC__) || defined(__clang__)
#    define LIBREMIDI_EXPORT __attribute__((visibility("default")))
#  endif
#else
#  define LIBREMIDI_EXPORT
#endif

#define LIBREMIDI_VERSION "1.0.0"

namespace libremidi
{
//! Defines various error types.
enum midi_error
{
  WARNING,           /*!< A non-critical error. */
  UNSPECIFIED,       /*!< The default, unspecified error type. */
  NO_DEVICES_FOUND,  /*!< No devices found on system. */
  INVALID_DEVICE,    /*!< An invalid device ID was specified. */
  MEMORY_ERROR,      /*!< An error occured during memory allocation. */
  INVALID_PARAMETER, /*!< An invalid parameter was specified to a function. */
  INVALID_USE,       /*!< The function was called incorrectly. */
  DRIVER_ERROR,      /*!< A system driver error occured. */
  SYSTEM_ERROR,      /*!< A system error occured. */
  THREAD_ERROR       /*!< A thread error occured. */
};

//! Base exception class for MIDI problems
struct LIBREMIDI_EXPORT midi_exception : public std::runtime_error
{
  using std::runtime_error::runtime_error;
  ~midi_exception() override;
};

struct LIBREMIDI_EXPORT no_devices_found_error final : public midi_exception
{
  static constexpr auto code = midi_error::NO_DEVICES_FOUND;
  using midi_exception::midi_exception;
  ~no_devices_found_error() override;
};
struct LIBREMIDI_EXPORT invalid_device_error final : public midi_exception
{
  static constexpr auto code = midi_error::INVALID_DEVICE;
  using midi_exception::midi_exception;
  ~invalid_device_error() override;
};
struct LIBREMIDI_EXPORT memory_error final : public midi_exception
{
  static constexpr auto code = midi_error::MEMORY_ERROR;
  using midi_exception::midi_exception;
  ~memory_error() override;
};
struct LIBREMIDI_EXPORT invalid_parameter_error final : public midi_exception
{
  static constexpr auto code = midi_error::INVALID_PARAMETER;
  using midi_exception::midi_exception;
  ~invalid_parameter_error() override;
};
struct LIBREMIDI_EXPORT invalid_use_error final : public midi_exception
{
  static constexpr auto code = midi_error::INVALID_USE;
  using midi_exception::midi_exception;
  ~invalid_use_error() override;
};
struct LIBREMIDI_EXPORT driver_error final : public midi_exception
{
  static constexpr auto code = midi_error::DRIVER_ERROR;
  using midi_exception::midi_exception;
  ~driver_error() override;
};
struct LIBREMIDI_EXPORT system_error final : public midi_exception
{
  static constexpr auto code = midi_error::SYSTEM_ERROR;
  using midi_exception::midi_exception;
  ~system_error() override;
};
struct LIBREMIDI_EXPORT thread_error final : public midi_exception
{
  static constexpr auto code = midi_error::THREAD_ERROR;
  using midi_exception::midi_exception;
  ~thread_error() override;
};

/*! \brief Error callback function
    \param type Type of error.
    \param errorText Error description.

    Note that class behaviour is undefined after a critical error (not
    a warning) is reported.
 */
using midi_error_callback = std::function<void(midi_error type, std::string_view errorText)>;

//! A static function to determine the current version.
std::string get_version() noexcept;

//! The callbacks will be called whenever a device is added or removed
//! for a given API.
class LIBREMIDI_EXPORT observer
{
public:
  struct callbacks
  {
    std::function<void(int, std::string)> input_added;
    std::function<void(int, std::string)> input_removed;
    std::function<void(int, std::string)> output_added;
    std::function<void(int, std::string)> output_removed;
  };

  observer(libremidi::API, callbacks);
  ~observer();

private:
  std::unique_ptr<class observer_api> impl_;
};

/**
 * Used to determine how large sent messages will be chunked.
 */
struct LIBREMIDI_EXPORT chunking_parameters {
  std::chrono::milliseconds interval{};
  int32_t size{};

  /**
   * @brief Will be called by the chunking code to allow the API user to wait.
   *
   * By default just calls sleep.
   * Arguments are: the time that must be waited, the bytes currently written.
   * Return false if you want to abort the transfer, and true otherwise.
   */
  std::function<bool(std::chrono::microseconds, int)> wait = chunking_parameters::default_wait;

  static bool default_wait(std::chrono::microseconds time_to_wait, int written_bytes);
};

/**********************************************************************/
/*! \class midi_in
    \brief A realtime MIDI input class.

    This class provides a common, platform-independent API for
    realtime MIDI input.  It allows access to a single MIDI input
    port.  Incoming MIDI messages are either saved to a queue for
    retrieval using the getMessage() function or immediately passed to
    a user-specified callback function.  Create multiple instances of
    this class to connect to more than one MIDI device at the same
    time.  With the OS-X, Linux ALSA, and JACK MIDI APIs, it is also
    possible to open a virtual input port to which other MIDI software
    clients can connect.

    by Gary P. Scavone, 2003-2017.
*/
class LIBREMIDI_EXPORT midi_in
{
public:
  //! User callback function type definition.
  using message_callback = std::function<void(const message& message)>;

  //! Default constructor that allows an optional api, client name and queue
  //! size.
  /*!
    An exception will be thrown if a MIDI system initialization
    error occurs.  The queue size defines the maximum number of
    messages that can be held in the MIDI queue (when not using a
    callback function).  If the queue size limit is reached,
    incoming messages will be ignored.

    If no API argument is specified and multiple API support has been
    compiled, the default order of use is ALSA, JACK (Linux) and CORE,
    JACK (OS-X).

    \param api        An optional API id can be specified.
    \param clientName An optional client name can be specified. This
                      will be used to group the ports that are created
                      by the application.
    \param queueSizeLimit An optional size of the MIDI input queue can be
    specified.
  */
  midi_in(
      libremidi::API api = API::UNSPECIFIED,
      std::string_view clientName = "libremidi input client",
      unsigned int queueSizeLimit = 100);

  //! If a MIDI connection is still open, it will be closed by the destructor.
  ~midi_in();

  //! Returns the MIDI API specifier for the current instance of midi_in.
  libremidi::API get_current_api() const noexcept;

  //! Open a MIDI input connection given by enumeration number.
  /*!
    \param portNumber A port number greater than 0 can be specified.
                      Otherwise, the default or first port found is opened.
    \param portName A name for the application port that is used to
    connect to portId can be specified.
  */
  void open_port(unsigned int portNumber, std::string_view portName);
  void open_port()
  {
    open_port(0, "libremidi Input");
  }
  void open_port(unsigned int port)
  {
    open_port(port, "libremidi Input");
  }

  //! Create a virtual input port, with optional name, to allow software
  //! connections (OS X, JACK and ALSA only).
  /*!
    This function creates a virtual MIDI input port to which other
    software applications can connect.  This type of functionality
    is currently only supported by the Macintosh OS-X, any JACK,
    and Linux ALSA APIs (the function returns an error for the other APIs).

    \param portName An optional name for the application port that is
                    used to connect to portId can be specified.
  */
  void open_virtual_port(std::string_view portName);
  void open_virtual_port()
  {
    open_virtual_port("libremidi virtual port");
  }
  //! Set a callback function to be invoked for incoming MIDI messages.
  /*!
    The callback function will be called whenever an incoming MIDI
    message is received.  While not absolutely necessary, it is best
    to set the callback function before opening a MIDI port to avoid
    leaving some messages in the queue.

    \param callback A callback function must be given.
  */
  void set_callback(message_callback callback);

  //! Cancel use of the current callback function (if one exists).
  /*!
    Subsequent incoming MIDI messages will be written to the queue
    and can be retrieved with the \e getMessage function.
  */
  void cancel_callback();

  //! Close an open MIDI connection (if one exists).
  void close_port();

  //! Returns true if a port is open and false if not.
  /*!
      Note that this only applies to connections made with the openPort()
      function, not to virtual ports.
  */
  bool is_port_open() const noexcept;

  //! Return the number of available MIDI input ports.
  /*!
    \return This function returns the number of MIDI ports of the selected API.
  */
  unsigned int get_port_count();

  //! Return a string identifier for the specified MIDI input port number.
  /*!
    \return The name of the port with the given Id is returned.
            An empty string is returned if an invalid port specifier
            is provided. User code should assume a UTF-8 encoding.
  */
  std::string get_port_name(unsigned int portNumber = 0);

  //! Specify whether certain MIDI message types should be queued or ignored
  //! during input.
  /*!
    By default, MIDI timing and active sensing messages are ignored
    during message input because of their relative high data rates.
    MIDI sysex messages are ignored by default as well.  Variable
    values of "true" imply that the respective message type will be
    ignored.
  */
  void ignore_types(bool midiSysex = true, bool midiTime = true, bool midiSense = true);

  //! Fill the user-provided vector with the data bytes for the next available
  //! MIDI message in the input queue and return the event delta-time in
  //! seconds.
  /*!
    This function returns immediately whether a new message is
    available or not.  A valid message is indicated by a non-zero
    vector size.  An exception is thrown if an error occurs during
    message retrieval or an input connection was not previously
    established.
  */
  message get_message();

  bool get_message(message&);

  //! Set an error callback function to be invoked when an error has occured.
  /*!
    The callback function will be called whenever an error has occured. It is
    best to set the error callback function before opening a port.
  */
  void set_error_callback(midi_error_callback errorCallback);

  void set_client_name(std::string_view clientName);

  void set_port_name(std::string_view portName);

private:
  std::unique_ptr<class midi_in_api> rtapi_;
};

/**********************************************************************/
/*! \class midi_out
    \brief A realtime MIDI output class.

    This class provides a common, platform-independent API for MIDI
    output.  It allows one to probe available MIDI output ports, to
    connect to one such port, and to send MIDI bytes immediately over
    the connection.  Create multiple instances of this class to
    connect to more than one MIDI device at the same time.  With the
    OS-X, Linux ALSA and JACK MIDI APIs, it is also possible to open a
    virtual port to which other MIDI software clients can connect.

    by Gary P. Scavone, 2003-2017.
*/
/**********************************************************************/

class LIBREMIDI_EXPORT midi_out
{
public:
  //! Default constructor that allows an optional client name.
  /*!
    An exception will be thrown if a MIDI system initialization error occurs.

    If no API argument is specified and multiple API support has been
    compiled, the default order of use is ALSA, JACK (Linux) and CORE,
    JACK (OS-X).
  */
  midi_out(libremidi::API api, std::string_view clientName);

  midi_out() : midi_out{libremidi::API::UNSPECIFIED, "libremidi client"}
  {
  }

  //! The destructor closes any open MIDI connections.
  ~midi_out();

  //! Returns the MIDI API specifier for the current instance of midi_out.
  libremidi::API get_current_api() noexcept;

  //! Open a MIDI output connection.
  /*!
      An optional port number greater than 0 can be specified.
      Otherwise, the default or first port found is opened.  An
      exception is thrown if an error occurs while attempting to make
      the port connection.
  */
  void open_port(unsigned int portNumber, std::string_view portName);
  void open_port()
  {
    open_port(0, "libremidi Output");
  }
  void open_port(unsigned int port)
  {
    open_port(port, "libremidi Output");
  }

  //! Close an open MIDI connection (if one exists).
  void close_port();

  //! Returns true if a port is open and false if not.
  /*!
      Note that this only applies to connections made with the openPort()
      function, not to virtual ports.
  */
  bool is_port_open() const noexcept;

  //! Create a virtual output port, with optional name, to allow software
  //! connections (OS X, JACK and ALSA only).
  /*!
      This function creates a virtual MIDI output port to which other
      software applications can connect.  This type of functionality
      is currently only supported by the Macintosh OS-X, Linux ALSA
      and JACK APIs (the function does nothing with the other APIs).
      An exception is thrown if an error occurs while attempting to
      create the virtual port.
  */
  void open_virtual_port(std::string_view portName);
  void open_virtual_port()
  {
    open_virtual_port("libremidi virtual port");
  }

  //! Return the number of available MIDI output ports.
  unsigned int get_port_count();

  //! Return a string identifier for the specified MIDI port type and number.
  /*!
    \return The name of the port with the given Id is returned.
            An empty string is returned if an invalid port specifier
            is provided. User code should assume a UTF-8 encoding.
  */
  std::string get_port_name(unsigned int portNumber = 0);

  //! Immediately send a single message out an open MIDI output port.
  /*!
      An exception is thrown if an error occurs during output or an
      output connection was not previously established.
  */
  void send_message(const std::vector<unsigned char>& message);

  void send_message(const libremidi::message& message);

  //! Immediately send a single message out an open MIDI output port.
  /*!
      An exception is thrown if an error occurs during output or an
      output connection was not previously established.

      \param message A pointer to the MIDI message as raw bytes
      \param size    Length of the MIDI message in bytes
  */
  void send_message(const unsigned char* message, size_t size);

  #if LIBREMIDI_HAS_SPAN
  void send_message(std::span<unsigned char>);
  #endif

  //! Set an error callback function to be invoked when an error has occured.
  /*!
    The callback function will be called whenever an error has occured. It is
    best to set the error callback function before opening a port.
  */
  void set_error_callback(midi_error_callback errorCallback) noexcept;

  void set_client_name(std::string_view clientName);

  void set_port_name(std::string_view portName);

  /**
   * For large messages, chunk their content and wait.
   * Setting a null optional will disable chunking.
   */
  void set_chunking_parameters(std::optional<chunking_parameters> parameters);

private:
  std::unique_ptr<class midi_out_api> rtapi_;
};
}

#if defined(LIBREMIDI_HEADER_ONLY)
#  include <libremidi/libremidi.cpp>
#endif
