#include <cas/simulator.hpp>

int main() {
  cas::Time::setResolutionTime(cas::Time::S);
  cas::Time::setUpdateDuration(1000);

  unsigned int brakes_consumed = 0;

  cas::OutsourceBench o1(15, 5, "tires");
  cas::OutsourceBench o2(60, 1, "engine");
  cas::OutsourceBench o3(30, 3, "brakes");

  cas::AssemblyBench a1(30, [&]() {
    unsigned int available = *o3.m_count - brakes_consumed;
    if (available >= 4) {
      brakes_consumed += 2;
      return true;
    }
    return false;
  });

  cas::Simulator::run();

  return 0;
}
