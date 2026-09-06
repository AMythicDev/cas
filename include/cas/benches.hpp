#pragma once

#include "cas/real-value.hpp"
#include <cas/simulator.hpp>
#include <cas/time.hpp>
#include <functional>
#include <optional>
#include <print>
#include <string>

namespace cas {
class OutsourceBench : public TickBench {
    unsigned int batch;
    std::string m_product;

  public:
    using next_t = std::function<void(unsigned int, RealValue<unsigned int>)>;

  private:
    std::optional<next_t> next;

  public:
    RealValue<unsigned int> m_count;

    OutsourceBench(simtime_t arrival_time, unsigned int batch, std::string product,
                   std::optional<next_t> next = std::nullopt, unsigned int count = 0)
        : TickBench(arrival_time), batch(batch), m_product(product), next(next), m_count(count) {}

    void processTick() override {
        m_count = *m_count + batch;
        if (next.has_value()) {
            next.value()(batch, m_count);
        }
        std::println("imported {} {} - available: {}", batch, m_product, *m_count + batch);
    }
};

class AssemblyBench : public TickBench {
    simtime_t completion_time;
    std::function<bool()> assemble;
    simtime_t check_after;
    bool processing = false;

  public:
    AssemblyBench(simtime_t completion_time, std::function<bool()> assemble, std::string name = "assembly bench",
                  simtime_t check_after = 1)
        : TickBench(check_after, check_after, name), completion_time(completion_time), assemble(assemble),
          check_after(check_after) {}

    void processTick() override {
        if (assemble()) {
            std::println("processing at {}", getName());
            processing = true;
            setRepeatTime(completion_time);
        } else if (processing && !assemble) {
            processing = false;
            setRepeatTime(check_after);
        }
    }
};

class ConveyorBench : public TickBench {
    enum class Type { EndToEnd, Circular };

    unsigned int placed_count = 0;
    unsigned int taken_count = 0;
    simtime_t delivery_time;

  public:
    RealValue<unsigned int> m_available = 0;
    ConveyorBench(simtime_t delivery_time) : TickBench(), delivery_time(delivery_time) {}
    ConveyorBench(simtime_t delivery_time, std::string name) : TickBench(name), delivery_time(delivery_time) {}

    void placeItem(unsigned int count) {
        placed_count += count;
        setTickTime(Simulator::getClock() + delivery_time);
    }

    bool takeItem(unsigned int count) {
        if (*m_available >= count) {
            taken_count += count;
            m_available = placed_count - taken_count;
            return true;
        } else {
            return false;
        }
    }

    void processTick() override {
        std::println("Delivered {}", getName());
        if (placed_count < taken_count) {
            m_available = 0;
            return;
        }
        m_available = placed_count - taken_count;
    }
};
} // namespace cas
