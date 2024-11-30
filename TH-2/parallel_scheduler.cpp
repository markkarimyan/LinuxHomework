#include "parallel_scheduler.h"

parallel_scheduler::parallel_scheduler(size_t capacity)
    : capacity(capacity), stop_flag(false) {
    for (size_t i = 0; i < capacity; ++i) {
        threads.push_back(std::thread(&parallel_scheduler::worker_thread, this));
    }
}

parallel_scheduler::~parallel_scheduler() {
    stop_flag = true;
    condition.notify_all();
    for (size_t i = 0; i < threads.size(); ++i) {
        if (threads[i].joinable()) {
            threads[i].join();
        }
    }
}

void parallel_scheduler::run(std::function<void()> func) {
    {
        std::lock_guard<std::mutex> lock(queue_mutex);
        task_queue.push(func);
    }
    condition.notify_one();
}

void parallel_scheduler::worker_thread() {
    while (!stop_flag) {
        std::function<void()> task;
        {
            std::unique_lock<std::mutex> lock(queue_mutex);
            condition.wait(lock, [this]() { return stop_flag || !task_queue.empty(); });

            if (stop_flag && task_queue.empty())
                return;

            task = task_queue.front();
            task_queue.pop();
        }
        task();
    }
}
