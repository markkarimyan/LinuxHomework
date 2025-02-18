#define _POSIX_C_SOURCE 200809L
#include <stdio.h>
#include <stdlib.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <unistd.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>

#define SEM_SMOKER_T "/sem_smoker_t"
#define SEM_SMOKER_P "/sem_smoker_p"
#define SEM_SMOKER_M "/sem_smoker_m"
#define SEM_BARTENDER "/sem_bartender"

volatile sig_atomic_t terminate_flag = 0;

void handle_sigterm(int signum) {
    terminate_flag = 1;
}

void smoker(char missing, sem_t *my_sem, sem_t *bartender_sem) {
    struct sigaction sa;
    sa.sa_handler = handle_sigterm;
    sigemptyset(&sa.sa_mask);
    sa.sa_flags = 0;
    if (sigaction(SIGTERM, &sa, NULL) == -1) {
        perror("sigaction");
        exit(1);
    }
    while (1) {
        while (sem_wait(my_sem) == -1) {
            if (errno == EINTR) {
                if (terminate_flag)
                    exit(0);
                continue;
            } else {
                perror("sem_wait");
                exit(1);
            }
        }
        if (terminate_flag)
            break;
        sigset_t block_set, old_set;
        sigemptyset(&block_set);
        sigaddset(&block_set, SIGTERM);
        if (sigprocmask(SIG_BLOCK, &block_set, &old_set) == -1) {
            perror("sigprocmask");
            exit(1);
        }
        if (putchar(missing) == EOF) {
            perror("putchar");
            exit(1);
        }
        fflush(stdout);
        if (sem_post(bartender_sem) == -1) {
            perror("sem_post");
            exit(1);
        }
        if (sigprocmask(SIG_SETMASK, &old_set, NULL) == -1) {
            perror("sigprocmask");
            exit(1);
        }
        if (terminate_flag)
            break;
    }
    exit(0);
}

int main(void) {
    sem_t *smoker_t_sem, *smoker_p_sem, *smoker_m_sem, *bartender_sem;
    if ((smoker_t_sem = sem_open(SEM_SMOKER_T, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
        (smoker_p_sem = sem_open(SEM_SMOKER_P, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
        (smoker_m_sem = sem_open(SEM_SMOKER_M, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED ||
        (bartender_sem = sem_open(SEM_BARTENDER, O_CREAT | O_EXCL, 0644, 0)) == SEM_FAILED) {
        perror("sem_open");
        exit(1);
    }
    pid_t pid_T, pid_P, pid_M;
    if ((pid_T = fork()) == 0) {
        smoker('T', smoker_t_sem, bartender_sem);
    } else if (pid_T < 0) {
        perror("fork");
        exit(1);
    }
    if ((pid_P = fork()) == 0) {
        smoker('P', smoker_p_sem, bartender_sem);
    } else if (pid_P < 0) {
        perror("fork");
        exit(1);
    }
    if ((pid_M = fork()) == 0) {
        smoker('M', smoker_m_sem, bartender_sem);
    } else if (pid_M < 0) {
        perror("fork");
        exit(1);
    }
    int ch;
    while ((ch = getchar()) != EOF) {
        switch(ch) {
            case 't':
                if (sem_post(smoker_t_sem) == -1) {
                    perror("sem_post");
                    exit(1);
                }
                break;
            case 'p':
                if (sem_post(smoker_p_sem) == -1) {
                    perror("sem_post");
                    exit(1);
                }
                break;
            case 'm':
                if (sem_post(smoker_m_sem) == -1) {
                    perror("sem_post");
                    exit(1);
                }
                break;
            default:
                continue;
        }
        if (sem_wait(bartender_sem) == -1) {
            perror("sem_wait");
            exit(1);
        }
    }
    kill(pid_T, SIGTERM);
    kill(pid_P, SIGTERM);
    kill(pid_M, SIGTERM);
    sem_post(smoker_t_sem);
    sem_post(smoker_p_sem);
    sem_post(smoker_m_sem);
    waitpid(pid_T, NULL, 0);
    waitpid(pid_P, NULL, 0);
    waitpid(pid_M, NULL, 0);
    sem_close(smoker_t_sem);
    sem_close(smoker_p_sem);
    sem_close(smoker_m_sem);
    sem_close(bartender_sem);
    sem_unlink(SEM_SMOKER_T);
    sem_unlink(SEM_SMOKER_P);
    sem_unlink(SEM_SMOKER_M);
    sem_unlink(SEM_BARTENDER);
    return 0;
}
