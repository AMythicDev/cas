#include <cas.hpp>
#include <print>
#include <thread>

namespace cas {
void Simulator::run() {
  while (true) {
    std::println("clock : {} sec", clock);
    if (job_queue.contains(clock)) {
      for (auto job : job_queue[clock]) {
        job->process();
      }
      jobs_count -= job_queue[clock].size();
      job_queue[clock].clear();
      job_queue.erase(clock);
    }

    if (jobs_count == 0) {
      std::println("--------- simulation stopped ---------");
      std::println("reason: no more jobs to run");
      return;
    }

    ++clock;
    std::this_thread::sleep_for(
        std::chrono::milliseconds(Time::getUpdateDuration()));
  }
}

} // namespace cas
