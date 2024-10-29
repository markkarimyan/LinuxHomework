#include <iostream>
#include <unistd.h>
#include <sys/wait.h>
#include <fcntl.h>
#include <sstream>
#include <vector>
#include <cstring>

std::vector<char *> parseCommand(const std::string &cmd) {
    std::istringstream iss(cmd);
    std::vector<char *> args;
    for (std::string arg; iss >> arg;) args.push_back(strdup(arg.c_str()));
    args.push_back(nullptr);
    return args;
}

int executeCommand(const std::string &cmd, bool silent, const std::string &outputFile, bool append) {
    auto args = parseCommand(cmd);
    pid_t pid = fork();
    if (pid == 0) {
        if (silent) {
            int log_fd = open((std::to_string(getpid()) + ".log").c_str(), O_WRONLY | O_CREAT | O_TRUNC, 0644);
            dup2(log_fd, STDOUT_FILENO); dup2(log_fd, STDERR_FILENO); close(log_fd);
        }
        if (!outputFile.empty()) {
            int out_fd = open(outputFile.c_str(), O_WRONLY | O_CREAT | (append ? O_APPEND : O_TRUNC), 0644);
            dup2(out_fd, STDOUT_FILENO); close(out_fd);
        }
        setenv("PATH", (std::string("./:") + getenv("PATH")).c_str(), 1);
        execvp(args[0], args.data()); perror("exec failed"); exit(1);
    } else if (pid > 0) {
        int status; waitpid(pid, &status, 0);
        for (char* arg : args) free(arg);
        return WIFEXITED(status) && WEXITSTATUS(status) == 0;
    }
    return false;
}

int main() {
    std::string command;
    while (true) {
        std::cout << "shell> ";
        std::getline(std::cin, command);
        if (command == "exit") break;

        std::istringstream iss(command);
        std::string token, outputFile, currentCommand;
        bool append = false, silent = false, executeNext = true, lastResult = true;

        while (iss >> token) {
            if (token == "&&" || token == "||" || token == ";") {
                if (executeNext && !currentCommand.empty()) {
                    lastResult = executeCommand(currentCommand, silent, outputFile, append);
                    currentCommand.clear();
                }
                executeNext = (token == ";" || (token == "&&" && lastResult) || (token == "||" && !lastResult));
                silent = false; outputFile.clear(); append = false;
                continue;
            }
            if (token == "silent") { silent = true; continue; }
            if (token.find(">") != std::string::npos) {
                append = (token.find(">>") != std::string::npos);
                outputFile = token.substr(append ? 2 : 1);
                continue;
            }
            currentCommand += token + " ";
        }
        if (executeNext && !currentCommand.empty()) {
            lastResult = executeCommand(currentCommand, silent, outputFile, append);
        }
    }
    return 0;
}
