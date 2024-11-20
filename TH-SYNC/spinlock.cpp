#include <iostream>
#include <pthread.h>
#include <chrono>
#include <fstream>

class Integer {
private:
    int inner_value = 0;
    volatile bool lock = false;
public:
    Integer(int value) : inner_value(value) {}
    void inc() {
        while (__sync_lock_test_and_set(&lock, 1)) {}
        inner_value++;
        __sync_lock_release(&lock);
    }
    int get_inner_value() {
        return inner_value;
    }
};

void* increment(void* arg) {
    Integer* num = (Integer*)arg;
    for (int i = 0; i < 10000; ++i) {
        num->inc();
    }
    return nullptr;
}

int main() {
    Integer num(0);
    int thread_count = 50;
    pthread_t tids[thread_count];

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < thread_count; ++i) {
        pthread_create(&tids[i], NULL, increment, &num);
    }

    for (int i = 0; i < thread_count; ++i) {
        pthread_join(tids[i], NULL);
    }

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> duration = end - start;

    std::ofstream out_file("spinlock-results.txt");
    out_file << "Final value: " << num.get_inner_value() << "\n";
    out_file << "Execution time with Spinlock: " << duration.count() << " seconds\n";
    out_file.close();

    return 0;
}