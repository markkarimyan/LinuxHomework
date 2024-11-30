#include "parallel_scheduler.h"
#include <iostream>
#include <chrono>

void sample_task(int id) {
    std::cout << "Task " << id << " started.\n";
    std::this_thread::sleep_for(std::chrono::milliseconds(500));
    std::cout << "Task " << id << " finished.\n";
}

int main() {
    const size_t pool_size = 4;
    parallel_scheduler scheduler(pool_size);

    for (int i = 1; i <= 10; ++i) {
        scheduler.run([i]() { sample_task(i); });
    }

    // Let tasks finish execution before exiting main
    std::this_thread::sleep_for(std::chrono::seconds(6));
    return 0;
}
