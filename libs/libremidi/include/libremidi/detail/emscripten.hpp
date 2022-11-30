#pragma once

#if defined(__EMSCRIPTEN__)
#include <libremidi/detail/emscripten_api.hpp>

namespace libremidi
{
namespace webmidi_helpers
{
class midi_access_emscripten
{
public:
  static midi_access_emscripten& instance() noexcept
  {
    static midi_access_emscripten inst;
    return inst;
  }

  const bool available() const noexcept
  {
    return EM_ASM_INT(return typeof globalThis.__libreMidi_access !== undefined;);
  }

  const int input_count() const noexcept
  {
    return EM_ASM_INT(return globalThis.__libreMidi_currentInputs.length;);
  }

  const int output_count() const noexcept
  {
    return EM_ASM_INT(return globalThis.__libreMidi_currentOutputs.length;);
  }

  const void load_current_infos() noexcept
  {
#define get_js_string(variable_to_read, ...) \
  (char*)EM_ASM_INT({ \
  var jsstr = variable_to_read; \
  var bytes = lengthBytesUTF8(jsstr) + 1; \
  var str = _malloc(bytes); \
  stringToUTF8(jsstr, str, bytes); \
  return str; \
}, __VA_ARGS__);


    EM_ASM_INT({
      let inputs = [];
      let outputs = [];
      for(let inpt of globalThis.__libreMidi_access.inputs.values()) {
        inputs.push(inpt);
      }
      for(let outpt of globalThis.__libreMidi_access.outputs.values()) {
        outputs.push(outpt);
      }
      globalThis.__libreMidi_currentInputs = inputs;
      globalThis.__libreMidi_currentOutputs = outputs;
    });
    const int inputs = input_count();
    const int outputs = output_count();

    m_current_inputs.resize(inputs);
    m_current_outputs.resize(outputs);

    for(int i = 0; i < inputs; i++)
    {
      int device_index = -1;
      char* midi_id = get_js_string(globalThis.__libreMidi_currentInputs[$0].id, i);
      auto it = m_input_indices.find(midi_id); // TODO transparent comparator, string_view...
      if(it == m_input_indices.end())
      {
        device_index = m_input_indices.size();
        m_input_indices[midi_id] = device_index;
      }
      else
      {
        device_index = it->second;
      }

      char* midi_name = get_js_string(globalThis.__libreMidi_currentInputs[$0].name, i);

      const bool connected = EM_ASM_INT(return globalThis.__libreMidi_currentInputs[$0].state === "connected", i);

      m_current_inputs[device_index].id = midi_id;
      m_current_inputs[device_index].name = midi_name;
      m_current_inputs[device_index].connected = connected;

      free(midi_id);
      free(midi_name);
    }

    for(int i = 0; i < outputs; i++)
    {
      int device_index = -1;
      char* midi_id = get_js_string(globalThis.__libreMidi_currentOutputs[$0].id, i);

      auto it = m_output_indices.find(midi_id); // TODO transparent comparator, string_view...
      if(it == m_output_indices.end())
      {
        device_index = m_output_indices.size();
        m_output_indices[midi_id] = device_index;
      }
      else
      {
        device_index = it->second;
      }

      char* midi_name = get_js_string(globalThis.__libreMidi_currentOutputs[$0].name, i);

      const bool connected = EM_ASM_INT(return globalThis.__libreMidi_currentOutputs[$0].state === "connected", i);

      m_current_outputs[device_index].id = midi_id;
      m_current_outputs[device_index].name = midi_name;
      m_current_outputs[device_index].connected = connected;

      free(midi_id);
      free(midi_name);
    }

#undef get_js_string
  }

  void register_observer(observer_emscripten& obs)
  {
    m_observers.push_back(&obs);
    if(m_observers.size() == 1)
    {
      start_observing();
    }
  }

  void unregister_observer(observer_emscripten& obs)
  {
    if(m_observers.size() == 1)
    {
      stop_observing();
    }
    auto it = std::find(m_observers.begin(), m_observers.end(), &obs);
    if(it != m_observers.end())
    {
      m_observers.erase(it);
    }
  }

  void devices_poll()
  {
    load_current_infos();

     for(auto& obs : m_observers)
    {
      obs->update(m_current_inputs, m_current_outputs);
    }
  }

  void open_input(int port_index, midi_in_emscripten& input)
  {
    auto& vec = m_opened_inputs[port_index];
    vec.push_back(&input);
    if(vec.size() != 1)
      return;

    start_stream(port_index);
  }

  void close_input(int port_index, midi_in_emscripten& input)
  {
    auto& vec = m_opened_inputs[port_index];
    auto it = std::find(vec.begin(), vec.end(), &input);
    if(it != vec.end())
    {
      vec.erase(it);
    }

    if(vec.empty())
    {
      stop_stream(port_index);
    }
  }

  void devices_input(int port, double timestamp, int len, char* data)
  {
    unsigned char* bytes = reinterpret_cast<unsigned char*>(data);
    message msg;
    msg.bytes.assign(bytes, bytes + len);
    msg.timestamp = timestamp;

    for(auto input : m_opened_inputs[port])
    {
      input->on_input(msg);
    }
  }

  void send_message(int port_index, const char* bytes, int len)
  {
    const auto& id = m_current_outputs[port_index].id;
    EM_ASM({
        let data = HEAPU8.subarray($0, $0 + $1);
        const id = UTF8ToString($2);
        let output = globalThis.__libreMidi_access.outputs.get(id);
        let bytes = HEAPU8.subarray($0, $0 + $1);
        output.send(Array.from(bytes));
    }, bytes, len, id.c_str());
  }

  const std::vector<device_information>& inputs() const noexcept { return m_current_inputs; }
  const std::vector<device_information>& outputs() const noexcept { return m_current_outputs; }

private:
  midi_access_emscripten() noexcept
  {
    EM_ASM(
     if(navigator.requestMIDIAccess) {
       navigator.requestMIDIAccess().then(
            (midiAccess) => globalThis.__libreMidi_access = midiAccess,
            () => console.log('MIDI support rejected, MIDI will not be available;'));
     } else {
       console.log('WebMIDI is not supported in this browser.');
     }
    );
  }

  ~midi_access_emscripten()
  {
    stop_observing();
  }

  void start_observing()
  {
    EM_ASM(
      let id = setInterval(Module._libremidi_devices_poll, 100);
      globalThis.__libreMidi_timer = id;
    );
  }

  void stop_observing()
  {
    EM_ASM(
      clearInterval(globalThis.__libreMidi_timer);
      globalThis.__libreMidi_timer = undefined;
    );
  }

  void start_stream(int port_index)
  {
    // Isn't life great...
    // https://github.com/Planeshifter/emscripten-examples/tree/master/01_PassingArrays
    const auto& id = m_current_inputs[port_index].id;
    EM_ASM(
      const port_index = $0;
      const id = UTF8ToString($1);

      let input = globalThis.__libreMidi_access.inputs.get(id);

      function _arrayToHeap(typedArray){
        const numBytes = typedArray.length * typedArray.BYTES_PER_ELEMENT;
        const ptr = Module._malloc(numBytes);
        const heapBytes = new Uint8Array(Module.HEAPU8.buffer, ptr, numBytes);
        heapBytes.set(new Uint8Array(typedArray.buffer));
        return heapBytes;
      }

      function _freeArray(heapBytes){
        Module._free(heapBytes.byteOffset);
      }

      input.onmidimessage = (message) => {
        let bytes = message.data;
        var heapBytes = _arrayToHeap(bytes);
        Module._libremidi_devices_input(port_index, message.timeStamp, bytes.length, heapBytes.byteOffset);
        _freeArray(heapBytes);
      };
    , port_index
    , id.c_str()
    );
  }

  void stop_stream(int port_index)
  {
    const auto& id = m_current_inputs[port_index].id;
    EM_ASM(
      const id = UTF8ToString($1);

      let input = globalThis.__libreMidi_access.inputs.get(id);
      input.onmidimessage = undefined;
    , id.c_str()
    );
  }

  std::vector<observer_emscripten*> m_observers;
  std::vector<device_information> m_current_inputs;
  std::vector<device_information> m_current_outputs;

  std::map<int, std::vector<midi_in_emscripten*>> m_opened_inputs;

  std::map<std::string, int> m_input_indices;
  std::map<std::string, int> m_output_indices;
};

}

struct emscripten_backend
{
  using midi_in = midi_in_emscripten;
  using midi_out = midi_out_emscripten;
  using midi_observer = observer_emscripten;
  static const constexpr auto API = libremidi::API::EMSCRIPTEN_WEBMIDI;
};

// Some implementation goes there
/// Observer ///
inline observer_emscripten::observer_emscripten(observer::callbacks&& c)
  : observer_api{std::move(c)}
{
  webmidi_helpers::midi_access_emscripten::instance().register_observer(*this);
}

inline observer_emscripten::~observer_emscripten()
{
  webmidi_helpers::midi_access_emscripten::instance().unregister_observer(*this);
}

inline void observer_emscripten::update(
    const std::vector<observer_emscripten::device>& current_inputs,
    const std::vector<observer_emscripten::device>& current_outputs)
{
  // WebMIDI never remove inputs, it just marks them as disconnected.
  // At least in known browsers...
  assert(current_inputs.size() >= m_known_inputs.size());
  assert(current_outputs.size() >= m_known_outputs.size());

  for(std::size_t i = m_known_inputs.size(); i < current_inputs.size(); i++)
  {
    m_known_inputs.push_back(current_inputs[i]);
    callbacks_.input_added(i, m_known_inputs[i].name);
  }

  for(std::size_t i = m_known_outputs.size(); i < current_outputs.size(); i++)
  {
    m_known_outputs.push_back(current_outputs[i]);
    callbacks_.output_added(i, m_known_outputs[i].name);
  }
}

/// midi_in ///
inline midi_in_emscripten::midi_in_emscripten(std::string_view clientName, unsigned int queueSizeLimit)
  : midi_in_default<midi_in_emscripten>{nullptr, queueSizeLimit}
{
}

inline midi_in_emscripten::~midi_in_emscripten()
{
  // Close a connection if it exists.
  midi_in_emscripten::close_port();
}

inline libremidi::API midi_in_emscripten::get_current_api() const noexcept
{
  return libremidi::API::EMSCRIPTEN_WEBMIDI;
}

inline void midi_in_emscripten::open_port(unsigned int portNumber, std::string_view)
{
  if (connected_)
  {
    warning("midi_in_emscripten::open_port: a valid connection already exists.");
    return;
  }

  auto& midi = webmidi_helpers::midi_access_emscripten::instance();

  if (portNumber >= midi.input_count())
  {
    error<no_devices_found_error>("midi_in_emscripten::open_port: no MIDI output sources found.");
    return;
  }

  midi.open_input(portNumber, *this);
  portNumber_ = portNumber;
  connected_ = true;
}

inline void midi_in_emscripten::close_port()
{
  if (connected_)
  {
    auto& midi = webmidi_helpers::midi_access_emscripten::instance();

    midi.close_input(portNumber_, *this);
    connected_ = false;
  }
}

inline void midi_in_emscripten::set_client_name(std::string_view clientName)
{
  warning("midi_in_emscripten::set_client_name: unsupported.");
}

inline void midi_in_emscripten::set_port_name(std::string_view portName)
{
  warning("midi_in_emscripten::set_port_name: unsupported.");
}

inline unsigned int midi_in_emscripten::get_port_count()
{
  return webmidi_helpers::midi_access_emscripten::instance().input_count();
}

inline std::string midi_in_emscripten::get_port_name(unsigned int portNumber)
{
  return webmidi_helpers::midi_access_emscripten::instance().inputs()[portNumber].name;
}

inline void midi_in_emscripten::on_input(message msg)
{
  this->inputData_.on_message_received(std::move(msg));
}


/// midi_out ///
inline midi_out_emscripten::midi_out_emscripten(std::string_view)
{
}

inline midi_out_emscripten::~midi_out_emscripten()
{
  // Close a connection if it exists.
  midi_out_emscripten::close_port();
}

inline libremidi::API midi_out_emscripten::get_current_api() const noexcept
{
  return libremidi::API::EMSCRIPTEN_WEBMIDI;
}

inline void midi_out_emscripten::open_port(unsigned int portNumber, std::string_view)
{
  if (connected_)
  {
    warning("midi_out_emscripten::open_port: a valid connection already exists.");
    return;
  }

  auto& midi = webmidi_helpers::midi_access_emscripten::instance();

  if (portNumber >= midi.output_count())
  {
    error<no_devices_found_error>("midi_out_emscripten::open_port: no MIDI output sources found.");
    return;
  }

  portNumber_ = portNumber;
  connected_ = true;
}

inline void midi_out_emscripten::close_port()
{
  if (connected_)
  {
    auto& midi = webmidi_helpers::midi_access_emscripten::instance();

    connected_ = false;
  }
}

inline void midi_out_emscripten::set_client_name(std::string_view clientName)
{
  warning("midi_out_emscripten::set_client_name: unsupported.");
}

inline void midi_out_emscripten::set_port_name(std::string_view portName)
{
  warning("midi_out_emscripten::set_port_name: unsupported.");
}

inline unsigned int midi_out_emscripten::get_port_count()
{
  return webmidi_helpers::midi_access_emscripten::instance().output_count();
}

inline std::string midi_out_emscripten::get_port_name(unsigned int portNumber)
{
  return webmidi_helpers::midi_access_emscripten::instance().outputs()[portNumber].name;
}

inline void midi_out_emscripten::send_message(const unsigned char* message, size_t size)
{
  if (!connected_)
    error<invalid_use_error>(
          "midi_out_emscripten::send_message: trying to send a message without an open port.");

  webmidi_helpers::midi_access_emscripten::instance().send_message(portNumber_, reinterpret_cast<const char*>(message), size);
}
}


extern "C"
{
inline void libremidi_devices_poll()
{
  libremidi::webmidi_helpers::midi_access_emscripten::instance().devices_poll();
}
inline void libremidi_devices_input(int port, double timestamp, int len, char* bytes)
{
  libremidi::webmidi_helpers::midi_access_emscripten::instance().devices_input(port, timestamp, len, bytes);
}
}
#endif

