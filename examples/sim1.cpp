#include <cas.hpp>

int main() {
  cas::Time::setResolutionTime(cas::Time::S);
  cas::Time::setUpdateDuration(1000);

  cas::TickBench tf1(1, 1);
  cas::TickBench tf2(10);

  cas::Simulator::run();

  return 0;
}
