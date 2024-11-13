#include <iostream>
#include <vector>
#include <thread>
#include <numeric>
#include <chrono>
#include <cstdlib>

void sum_part(const std::vector<int>& arr, int start, int end, long long &partial_sum) {
    partial_sum = std::accumulate(arr.begin() + start, arr.begin() + end, 0LL);
}

int main(int argc, char* argv[]) {
    if (argc < 3) {
        std::cerr << "Usage: " << argv[0] << " <array_size> <num_threads>" << std::endl;
        return 1;
    }

    int N = std::atoi(argv[1]);
    int M = std::atoi(argv[2]);

    if (N <= 1000000 || M < 1) {
        std::cerr << "Please enter N > 1000000 and M >= 1" << std::endl;
        return 1;
    }

    std::vector<int> arr(N);
    for (int i = 0; i < N; ++i) {
        arr[i] = std::rand() % 100;
    }

    auto start = std::chrono::high_resolution_clock::now();
    long long single_thread_sum = std::accumulate(arr.begin(), arr.end(), 0LL);
    auto end = std::chrono::high_resolution_clock::now();
    auto duration_single = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time spent without threads: " << duration_single << " ms" << std::endl;

    start = std::chrono::high_resolution_clock::now();
    std::vector<std::thread> threads;
    std::vector<long long> partial_sums(M, 0);
    int chunk_size = N / M;

    for (int i = 0; i < M; ++i) {
        int start_idx = i * chunk_size;
        int end_idx = (i == M - 1) ? N : start_idx + chunk_size;
        threads.emplace_back(sum_part, std::cref(arr), start_idx, end_idx, std::ref(partial_sums[i]));
    }

    for (auto &t : threads) {
        t.join();
    }

    long long multi_thread_sum = std::accumulate(partial_sums.begin(), partial_sums.end(), 0LL);
    end = std::chrono::high_resolution_clock::now();
    auto duration_multi = std::chrono::duration_cast<std::chrono::milliseconds>(end - start).count();
    std::cout << "Time spent with " << M << " threads: " << duration_multi << " ms" << std::endl;

    return 0;
}