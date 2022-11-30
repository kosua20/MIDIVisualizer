#pragma once
#include <memory>
#include <libremidi/message.hpp>

namespace libremidi
{

struct midi_queue
{
  unsigned int front{};
  unsigned int back{};
  unsigned int ringSize{};
  std::unique_ptr<message[]> ring{};

  bool push(const message& msg)
  {
    auto [sz, _, b] = get_dimensions();

    if (sz < ringSize - 1)
    {
      ring[b] = msg;
      back = (back + 1) % ringSize;
      return true;
    }

    return false;
  }

  bool pop(message& msg)
  {
    auto [sz, f, _] = get_dimensions();

    if (sz == 0)
    {
      return false;
    }

    // Copy queued message to the vector pointer argument and then "pop" it.
    using namespace std;
    swap(msg, ring[f]);

    // Update front
    front = (front + 1) % ringSize;
    return true;
  }

  struct dimensions
  {
    unsigned int size, front, back;
  };

  dimensions get_dimensions() const
  {
    // Access back/front members exactly once and make stack copies for
    // size calculation ==> completely unneccessary
    // https://godbolt.org/g/HPu9LA

    return {(back >= front) ? back - front : ringSize - front + back, front, back};
  }
};

}
