#include <iostream>
#include <pthread.h>
#include <chrono>
#include <fstream>

class Integer {
private:
    int inner_value = 0;
    pthread_mutex_t mtx;
public:
    Integer(int value) : inner_value(value) {
        pthread_mutex_init(&mtx, NULL);
    }
    void inc() {
        pthread_mutex_lock(&mtx);
        inner_value++;
        pthread_mutex_unlock(&mtx);
    }
    int get_inner_value() {
        return inner_value;
    }
    ~Integer() {
        pthread_mutex_destroy(&mtx);
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

    std::ofstream out_file("mutex-results.txt");
    out_file << "Final value: " << num.get_inner_value() << "\n";
    out_file << "Execution time with Mutex: " << duration.count() << " seconds\n";
    out_file.close();

    return 0;
}