#pragma once

#include <map>
#include <print>
#include <string>
#include <vector>

namespace cas {
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
  static long long getUpdateDuration() { return m_update; }
  static Unit getResolutionTime() { return m_res; }

private:
  inline static Unit m_res;
  inline static long long m_update;
};

class Workbench {
public:
  virtual std::string getName() { return "Workbench"; }
  virtual void process() {}
};

class Simulator {
  static inline unsigned long long clock = 0;
  static inline std::map<unsigned long long, std::vector<Workbench *>>
      job_queue;
  static inline unsigned int jobs_count = 0;

public:
  static void run();
  static unsigned long long getClock() { return clock; }
  static void insertJobQueue(Workbench *wb, unsigned long long time) {
    job_queue[time].push_back(wb);
    jobs_count++;
  }
};

class TickBench : public Workbench {
  unsigned long long repeat;
  unsigned long long offset;

public:
  TickBench(unsigned long long offset, unsigned long long repeat = 0)
      : repeat(repeat), offset(offset) {
    Simulator::insertJobQueue(this, offset);
  }

  void process() override {
    if (Simulator::getClock() == offset) {
      std::print("tick bench executed at {}", offset + repeat);
    }
    if (repeat != 0) {
      offset = Simulator::getClock();
      std::print(", next tick scheduled at {}", offset + repeat);
      Simulator::insertJobQueue(this, offset + repeat);
      offset += repeat;
    }
    std::print("\n");
  }
};

} // namespace cas
