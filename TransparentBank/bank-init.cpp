#include <iostream>
#include <vector>
#include <sys/mman.h>
#include <fcntl.h>
#include <unistd.h>

struct Account {
    int balance;
    int min_balance;
    int max_balance;
    bool frozen;
};

class Bank {
public:
    Bank(int n) : num_accounts(n) {
        for (int i = 0; i < n; ++i) {
            accounts[i] = {0, 0, 10000, false};
        }
    }


    // void printAccount(int index) {
    //     if (index < 0 || index >= num_accounts) {
    //         std::cout << "Invalid account index\n";
    //         return;
    //     }
    //     std::cout << "Account " << index << " Balance: " << accounts[index].balance << " Frozen: " << accounts[index].frozen << "\n";
    // }

private:
    Account* accounts;
    int num_accounts;
};

int main(int n, char** argv) {
    if (n != 2) {
        perror("Wrong nubmer of arguments");
        exit(1);
    }
    const char *name = "/bank_shm";
    int fd = shm_open(name, O_CREAT | O_RDWR, 0666);
    if(fd == -1){
          perror("shm_open");
          exit(EXIT_FAILURE);
    }
    int shm_size = sizeof(Bank) + n * sizeof(Account);
    ftruncate(fd, shm_size);
    void* ptr = mmap(0, shm_size, PROT_WRITE | PROT_READ, MAP_SHARED, fd, 0);
    if(ptr == MAP_FAILED){
          perror("mmap");
          exit(EXIT_FAILURE);
    }
  
    Bank* bank_ptr = new (ptr) Bank(n);
  
    munmap(ptr, shm_size);
    close(fd);
    shm_unlink(name);
    return 0;
  
  }

