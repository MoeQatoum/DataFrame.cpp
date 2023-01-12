#include "assert.h"
#include <chrono>

using time_point = typename std::chrono::high_resolution_clock::time_point;

template<typename TimeUnit, class Clock = std::chrono::high_resolution_clock>
class Timer
{
  using time_point   = typename Clock::time_point;
  time_point m_start = Clock::now();
  time_point m_end   = {};

  public:
  void tick()
  {
    m_end   = time_point{};
    m_start = Clock::now();
  }

  void tock() { m_end = Clock::now(); }

  auto duration() const
  {
    assert(m_end != time_point{});
    return std::chrono::duration_cast<TimeUnit>(m_end - m_start);
  }
};
