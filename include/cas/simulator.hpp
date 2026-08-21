#pragma once

#include <cas/real-value.hpp>
#include <functional>
#include <map>
#include <print>
#include <string>
#include <vector>

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

class Workbench {
public:
  virtual std::string getName() { return "Workbench"; }
  virtual void process() {}
};

class Simulator {
  static inline simtime_t clock = 0;
  static inline std::map<simtime_t, std::vector<Workbench *>> job_queue;
  static inline unsigned int jobs_count = 0;

public:
  static void run();
  static simtime_t getClock() { return clock; }
  static void insertJobQueue(Workbench *wb, simtime_t time) {
    job_queue[time].push_back(wb);
    jobs_count++;
  }
  static void removeJob(Workbench *wb);
};

class TickBench : public Workbench {
  simtime_t repeat;
  simtime_t offset;

public:
  TickBench(simtime_t repeat) : repeat(repeat), offset(0) {
    Simulator::insertJobQueue(this, repeat);
  }

  TickBench(simtime_t offset, simtime_t repeat)
      : repeat(repeat), offset(offset) {
    Simulator::insertJobQueue(this, offset);
  }

  void setRepeatTime(simtime_t r) { repeat = r; }

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
  RealValue<unsigned int> m_count;

  OutsourceBench(simtime_t arrival_time, unsigned int batch,
                 std::string product, unsigned int count = 0)
      : TickBench(arrival_time), batch(batch), m_product(product),
        m_count(count) {}

  void processTick() override {
    m_count = *m_count + batch;
    std::println("imported {} {} - available: {}", batch, m_product,
                 *m_count + batch);
  }
};

class AssemblyBench : public TickBench {
  simtime_t completion_time;
  std::function<bool()> assemble;
  simtime_t check_after;
  bool processing = false;

public:
  AssemblyBench(simtime_t completion_time, std::function<bool()> assemble,
                simtime_t check_after = 1)
      : TickBench(check_after), completion_time(completion_time),
        assemble(assemble), check_after(check_after) {}

  void processTick() override {
    std::println("check at assembly bench");
    if ((!processing && assemble()) || (processing && assemble)) {
      std::println("processing at assembly bench");
      processing = true;
      setRepeatTime(completion_time);
    } else if (processing && !assemble) {
      processing = false;
      setRepeatTime(check_after);
    }
  }
};

} // namespace cas
