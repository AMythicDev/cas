#include "cas/real-value.hpp"
#include <cas/benches.hpp>
#include <cas/simulator.hpp>

int main() {
    cas::Time::setResolutionTime(cas::Time::S);
    cas::Time::setUpdateDuration(100);

    unsigned int brakes_consumed = 0;

    cas::OutsourceBench o1(15, 5, "tires");
    cas::ConveyorBench c1(10, "engines");
    cas::OutsourceBench o2(60, 1, "engine",
                           [&](unsigned int batch, cas::RealValue<unsigned int> _) { c1.placeItem(batch); });
    cas::OutsourceBench o3(30, 3, "brakes");

    cas::AssemblyBench a1(
        30,
        [&]() {
            unsigned int available = *o3.m_count - brakes_consumed;
            if (available >= 4) {
                brakes_consumed += 2;
                return true;
            }
            return false;
        },
        "brakes processing");

    cas::AssemblyBench a2(10, [&]() { return c1.takeItem(1); }, "engine processing");

    cas::Simulator::run();

    return 0;
}
