#include <cas.hpp>

int main() {
  cas::Time::setResolutionTime(cas::Time::S);
  cas::Time::setUpdateDuration(1000);

  cas::OutsourceBench o1(15, 5, "tires");
  cas::OutsourceBench o2(60, 1, "engine");
  cas::OutsourceBench o3(30, 3, "brakes");

  cas::AssemblyBench a1(30, [&]() {
    if (o3.m_count >= 4) {
      o3.m_count -= 2;
      return true;
    }

    return false;
  });

  cas::Simulator::run();

  return 0;
}
