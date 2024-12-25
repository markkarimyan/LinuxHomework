#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>

pthread_mutex_t printerMutex;

void* usePrinter(void* arg) {
    int id = *((int*)arg);
    pthread_mutex_lock(&printerMutex);
    std::cout << "Thread " << id << " is using the printer." << std::endl;
    pthread_mutex_unlock(&printerMutex);
    return nullptr;
}

int main() {
    pthread_mutex_init(&printerMutex, nullptr);
    int numThreads;
    std::cout << "Enter the number of threads: ";
    std::cin >> numThreads;

    std::vector<pthread_t> threads(numThreads);
    std::vector<int> threadIds(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        threadIds[i] = i + 1;
        pthread_create(&threads[i], nullptr, usePrinter, (void*)&threadIds[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    pthread_mutex_destroy(&printerMutex);
    return 0;
}
