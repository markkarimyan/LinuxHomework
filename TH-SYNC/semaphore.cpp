#include <iostream>
#include <pthread.h>
#include <semaphore.h>
#include <chrono>
#include <fstream>

class Integer {
private:
    int inner_value = 0;
    sem_t sem;
public:
    Integer(int value) : inner_value(value) {
        sem_init(&sem, 0, 1);
    }
    void inc() {
        sem_wait(&sem);
        inner_value++;
        sem_post(&sem);
    }
    int get_inner_value() {
        return inner_value;
    }
    ~Integer() {
        sem_destroy(&sem);
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

    std::ofstream out_file("semaphore-results.txt");
    out_file << "Final value: " << num.get_inner_value() << "\n";
    out_file << "Execution time with Semaphore: " << duration.count() << " seconds\n";
    out_file.close();

    return 0;
}