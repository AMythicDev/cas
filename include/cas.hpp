#pragma once

#include <functional>
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
  TickBench(unsigned long long repeat = 0) : repeat(repeat), offset(repeat) {
    Simulator::insertJobQueue(this, repeat);
  }

  TickBench(unsigned long long offset, unsigned long long repeat)
      : repeat(repeat), offset(offset) {
    Simulator::insertJobQueue(this, offset);
  }

  void setRepeatTime(unsigned long long r) { repeat = r; }

  virtual void processTick() {
    std::print("tick bench executed at {}", offset);
  }

  void process() override {
    processTick();
    if (repeat != 0) {
      offset = Simulator::getClock();
      Simulator::insertJobQueue(this, offset + repeat);
      offset += repeat;
    }
  }
};

class OutsourceBench : public TickBench {
  unsigned int batch;
  std::string m_product;

public:
  unsigned int m_count;

  OutsourceBench(unsigned long long arrival_time, unsigned int batch,
                 std::string product, unsigned int count = 0)
      : TickBench(arrival_time), m_count(count), batch(batch),
        m_product(product) {}

  void processTick() override {
    m_count += batch;
    std::println("imported {} {} - available: {}", batch, m_product, m_count);
  }
};

class AssemblyBench : public TickBench {
  unsigned long long completion_time;
  std::function<bool()> requirements;
  bool processing = false;

public:
  AssemblyBench(unsigned long long completion_time,
                std::function<bool()> requirements)
      : TickBench(1), completion_time(completion_time),
        requirements(requirements) {}

  void processTick() override {
    std::println("check at assembly bench");
    if ((!processing && requirements()) || (processing && requirements)) {
      std::println("processing at assembly bench");
      processing = true;
      setRepeatTime(completion_time);
    } else if (processing && !requirements) {
      processing = false;
      setRepeatTime(1);
    }
  }
};

} // namespace cas
