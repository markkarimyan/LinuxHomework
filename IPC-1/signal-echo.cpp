#include <iostream>
#include <csignal>
#include <unistd.h>
#include <sys/types.h>
#include <pwd.h>
#include <ucontext.h>

void signal_handler(int signal, siginfo_t *info, void *context) {
    if (signal == SIGUSR1) {
        pid_t sender_pid = info->si_pid;
        uid_t sender_uid = info->si_uid;
        struct passwd *pwd = getpwuid(sender_uid);
        const char *username = pwd ? pwd->pw_name : "unknown";

        ucontext_t *uc = (ucontext_t *)context;
        unsigned long eip = uc->uc_mcontext.gregs[REG_RIP];
        unsigned long eax = uc->uc_mcontext.gregs[REG_RAX];
        unsigned long ebx = uc->uc_mcontext.gregs[REG_RBX];

        std::cout << "Received a SIGUSR1 signal from process " << sender_pid
                  << " executed by UID " << sender_uid
                  << " (" << username << ").\n"
                  << "State of the context: EIP = " << std::hex << eip
                  << ", EAX = " << std::hex << eax
                  << ", EBX = " << std::hex << ebx << ".\n";
    }
}

int main() {
    std::cout << "Signal Echo Program (PID: " << getpid() << ")\n";
    struct sigaction sa;
    sa.sa_sigaction = signal_handler;
    sa.sa_flags = SA_SIGINFO;
    sigaction(SIGUSR1, &sa, nullptr);

    while (true) {
        sleep(10);
    }

    return 0;
}
