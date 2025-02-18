#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/wait.h>
#include <sys/mman.h>

#define NUM_PHILOSOPHERS 5

void philosopher(int id, sem_t *sticks) {
    while (1) {
        printf("Philosopher %d is thinking.\n", id);
        sleep(rand() % 3 + 1);
        
        if (id == 0) {
            sem_wait(&sticks[(id + 1) % NUM_PHILOSOPHERS]);
            sem_wait(&sticks[id]);
        } else {
            sem_wait(&sticks[id]);
            sem_wait(&sticks[(id + 1) % NUM_PHILOSOPHERS]);
        }
        
        printf("Philosopher %d is eating.\n", id);
        sleep(rand() % 3 + 1);
        
        sem_post(&sticks[id]);
        sem_post(&sticks[(id + 1) % NUM_PHILOSOPHERS]);
        printf("Philosopher %d finished eating.\n\n", id);
    }
}

int main() {
    sem_t *sticks = (sem_t *)mmap(NULL, sizeof(sem_t) * NUM_PHILOSOPHERS,
                                  PROT_READ | PROT_WRITE,
                                  MAP_SHARED | MAP_ANONYMOUS, -1, 0);
    if (sticks == MAP_FAILED) {
        perror("mmap failed");
        exit(EXIT_FAILURE);
    }
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        if (sem_init(&sticks[i], 1, 1) < 0) {
            perror("sem_init failed");
            exit(EXIT_FAILURE);
        }
    }
    
    pid_t pids[NUM_PHILOSOPHERS];
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        pids[i] = fork();
        if (pids[i] < 0) {
            perror("fork failed");
            exit(EXIT_FAILURE);
        }
        if (pids[i] == 0) {
            srand(getpid());
            philosopher(i, sticks);
            exit(EXIT_SUCCESS);
        }
    }
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        wait(NULL);
    }
    
    for (int i = 0; i < NUM_PHILOSOPHERS; i++) {
        sem_destroy(&sticks[i]);
    }
    
    munmap(sticks, sizeof(sem_t) * NUM_PHILOSOPHERS);
    return 0;
}
