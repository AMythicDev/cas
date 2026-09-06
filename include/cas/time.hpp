#pragma once

namespace cas {
using simtime_t = unsigned long long;

class Time {
public:
  enum Unit : int {
    Y = 0,   // year, 365 days
    D = 1,   // day, 24 hours
    H = 2,   // hour, 60 minutes
    MIN = 3, // minute, 60 seconds
    S = 4,   // second
    MS = 5,  // millisecond
  };

  static void setResolutionTime(Unit res) { m_res = res; }
  static void setUpdateDuration(unsigned long long millis) {
    m_update = millis;
  }
  static unsigned long long getUpdateDuration() { return m_update; }
  static Unit getResolutionTime() { return m_res; }

private:
  inline static Unit m_res;
  inline static unsigned long long m_update;
};

} // namespace cas
