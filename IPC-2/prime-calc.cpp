#include <iostream>
#include <unistd.h>
#include <string>
#include <vector>
#include <sstream>

bool isPrime(int n) {
    if (n <= 1) return false;
    for (int i = 2; i * i <= n; ++i) {
        if (n % i == 0) return false;
    }
    return true;
}

int findMthPrime(int m) {
    int count = 0;
    int num = 1;
    while (count < m) {
        ++num;
        if (isPrime(num)) {
            ++count;
        }
    }
    return num;
}

int main() {
    int pipefd1[2];
    int pipefd2[2];

    if (pipe(pipefd1) == -1 || pipe(pipefd2) == -1) {
        perror("pipe");
        return 1;
    }

    pid_t pid = fork();

    if (pid < 0) {
        perror("fork");
        return 1;
    } else if (pid == 0) {
        close(pipefd1[1]);
        close(pipefd2[0]);

        while (true) {
            int m;
            read(pipefd1[0], &m, sizeof(m));

            if (m == -1) {
                break;
            }

            std::cout << "[Child] Calculating " << m << "-th prime number...\n";
            int result = findMthPrime(m);

            std::cout << "[Child] Sending calculation result of prime(" << m << ")...\n";
            write(pipefd2[1], &result, sizeof(result));
        }

        close(pipefd1[0]);
        close(pipefd2[1]);
    } else {
        close(pipefd1[0]);
        close(pipefd2[1]);

        while (true) {
            std::string input;
            std::cout << "[Parent] Please enter the number: ";
            std::cin >> input;

            if (input == "exit") {
                int exitSignal = -1;
                write(pipefd1[1], &exitSignal, sizeof(exitSignal));
                break;
            }

            int m;
            std::stringstream ss(input);
            ss >> m;

            std::cout << "[Parent] Sending " << m << " to the child process...\n";
            write(pipefd1[1], &m, sizeof(m));

            std::cout << "[Parent] Waiting for the response from the child process...\n";
            int result;
            read(pipefd2[0], &result, sizeof(result));

            std::cout << "[Parent] Received calculation result of prime(" << m << ") = " << result << "...\n";
        }

        close(pipefd1[1]);
        close(pipefd2[0]);
    }

    return 0;
}
