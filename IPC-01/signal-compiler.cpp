#include <iostream>
#include <csignal>
#include <unistd.h>
#include <atomic>

std::atomic<int> sigusr2_count(0);

void handle_signal(int signal) {
    if (signal == SIGUSR2) {
        sigusr2_count++;
    } else if (signal == SIGINT) {
        std::cout << "\nReceived SIGINT. Total SIGUSR2 signals: " << sigusr2_count.load() << std::endl;
        exit(0);
    }
}

int main() {

    signal(SIGUSR1, SIG_IGN);


    signal(SIGUSR2, handle_signal);
    signal(SIGINT, handle_signal);

    std::cout << "Program is running. Press Ctrl+C (SIGINT) to exit." << std::endl;

    while (true) {
        std::cout << "I am still alive!!" << std::endl;
        sleep(5);
    }

    return 0;
}
