#pragma once

#include <cas/real-value.hpp>
#include <cas/time.hpp>
#include <map>
#include <optional>
#include <print>
#include <string>

namespace cas {
class Workbench {
  private:
    std::string __wb_name = "Workbench";

  public:
    virtual const std::string &getName() const { return __wb_name; }
    virtual void process() {}
    Workbench() {}
    explicit Workbench(std::string name) : __wb_name(name) {}
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
    explicit TickBench() : repeat(0), offset(0) {}
    TickBench(std::string name) : Workbench(name), repeat(0), offset(0) {}

    TickBench(simtime_t repeat) : repeat(repeat), offset(0) { Simulator::insertJobQueue(this, repeat); }

    TickBench(simtime_t offset, simtime_t repeat) : repeat(repeat), offset(offset) {
        Simulator::insertJobQueue(this, offset);
    }

    TickBench(simtime_t offset, simtime_t repeat, std::string name) : Workbench(name), repeat(repeat), offset(offset) {
        Simulator::insertJobQueue(this, offset);
    }

    void setRepeatTime(simtime_t r) { repeat = r; }

    void setTickTime(simtime_t t) {
        offset = t;
        Simulator::insertJobQueue(this, offset);
    }

    virtual void processTick() { std::print("tick bench executed at {}", offset); }

    void process() override {
        processTick();
        if (repeat != 0) {
            offset = Simulator::getClock();
            Simulator::insertJobQueue(this, offset + repeat);
            offset += repeat;
        }
    }
};

} // namespace cas
