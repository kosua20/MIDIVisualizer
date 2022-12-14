#include <libremidi/libremidi.hpp>
#include <thread>
#include <vector>
#include <mutex>
#include <stop_token>
#include <deque>

/**
 * @file multithread_midiout.cpp
 *
 * The midi output is not thread-safe.
 * This file shows an example design to send messages from multiple threads synchronously.
 */


/** Note: instead of using this very naïve mutex-based queue, we recommend using either of
 *  https://github.com/max0x7ba/atomic_queue
 *  https://github.com/cameron314/concurrentqueue
 *  (or any other lockfree queue
 **/
template<typename T>
class threadsafe_queue
{
public:
  threadsafe_queue()
  {

  }

  void try_enqueue(T&& t)
  {
    std::scoped_lock lock{m_mutex};
    m_data.push_back(std::move(t));
  }

  bool try_dequeue(T& t)
  {
    std::scoped_lock lock{m_mutex};
    if(!m_data.empty())
    {
      t = std::move(m_data.front());
      m_data.pop_front();
      return true;
    }
    return false;
  }

private:
  std::mutex m_mutex;
  std::deque<T> m_data;
};

int main()
{
  using namespace std::literals;
  using clk = std::chrono::steady_clock;

  const int num_threads = 20;

  // Open our midi output
  libremidi::midi_out output{};
  if(output.get_port_count() == 0)
  {
    std::cerr << "No MIDI outputs are available" << std::endl;
    std::exit(1);
  }
  output.open_port();

  // Create a message queue for communication
  threadsafe_queue<libremidi::message> queue;
  auto midi_writer = [&] (std::stop_token stoken) {
    while (!stoken.stop_requested()) {
      queue.try_enqueue({176, 7, (unsigned char)(rand())});
      std::this_thread::sleep_for(std::chrono::milliseconds(5));
    }
  };

  // Create producer threads
  std::vector<std::jthread> output_threads;
  for(int i = 0; i < 20; i++)
    output_threads.emplace_back(midi_writer);

  // Consume the produced messages
  auto t0 = clk::now();

  int k = 0;
  libremidi::message msg;
  while((clk::now() - t0) < 5s)
  {
    if(queue.try_dequeue(msg))
    {
      output.send_message(msg);
      k++;
    }
  }
  output_threads.clear();

  // Process remaining messages - most of the time is actually spent inside OS APIs for sending MIDI messages,
  // and those aren't thread-safe so no way out of doing it that way.
  while(queue.try_dequeue(msg))
  {
    k++;
    output.send_message(msg);
  }

  std::cout << "Sent " << k
            << " messages over " << num_threads
            << " threads in " << std::chrono::duration_cast<std::chrono::milliseconds>(clk::now() - t0).count()
            << " milliseconds\n"
  ;
}
