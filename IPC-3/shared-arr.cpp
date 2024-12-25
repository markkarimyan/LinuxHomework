#include <iostream>
#include <fcntl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <unistd.h>
#include <cstring>
#include <semaphore.h>
#include <stdexcept>

class shared_array {
public:
    shared_array(const char* name, size_t size) : size_(size) {
        std::strcpy(shm_name_, "/");
        std::strcat(shm_name_, name);

        shm_fd_ = shm_open(shm_name_, O_CREAT | O_RDWR, 0666);
        if (shm_fd_ == -1) {
            perror("shm_open");
            throw std::runtime_error("Failed to create shared memory");
        }

        if (ftruncate(shm_fd_, size_ * sizeof(int)) == -1) {
            perror("ftruncate");
            throw std::runtime_error("Failed to set size of shared memory");
        }

        data_ = static_cast<int*>(mmap(nullptr, size_ * sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd_, 0));
        if (data_ == MAP_FAILED) {
            perror("mmap");
            throw std::runtime_error("Failed to map shared memory");
        }

        std::strcpy(sem_name_, "/sem_");
        std::strcat(sem_name_, name);
        semaphore_ = sem_open(sem_name_, O_CREAT, 0666, 1);
        if (semaphore_ == SEM_FAILED) {
            perror("sem_open");
            throw std::runtime_error("Failed to create semaphore");
        }
    }

    ~shared_array() {
        munmap(data_, size_ * sizeof(int));
        close(shm_fd_);
        shm_unlink(shm_name_);
        sem_close(semaphore_);
        sem_unlink(sem_name_);
    }

    int& operator[](size_t index) {
        if (index >= size_) {
            throw std::out_of_range("Index out of range");
        }
        return data_[index];
    }

    void lock() {
        sem_wait(semaphore_);
    }

    void unlock() {
        sem_post(semaphore_);
    }

private:
    char shm_name_[256];
    char sem_name_[256];
    size_t size_;
    int shm_fd_;
    int* data_;
    sem_t* semaphore_;
};

int main() {
    return 0;
}
