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
        int shm_fd = shm_open("/bank_shm", O_CREAT | O_RDWR, 0666);
        ftruncate(shm_fd, sizeof(Account) * n);
        accounts = static_cast<Account*>(mmap(0, sizeof(Account) * n, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0));
        for (int i = 0; i < n; ++i) {
            accounts[i] = {0, 0, 10000, false};
        }
    }

    ~Bank() {
        munmap(accounts, sizeof(Account) * num_accounts);
        shm_unlink("/bank_shm");
    }

    void printAccount(int index) {
        if (index < 0 || index >= num_accounts) {
            std::cout << "Invalid account index\n";
            return;
        }
        std::cout << "Account " << index << " Balance: " << accounts[index].balance << " Frozen: " << accounts[index].frozen << "\n";
    }

private:
    Account* accounts;
    int num_accounts;
};

int main(int argc, char* argv[]) {
    if (argc != 2) {
        std::cerr << "Usage: " << argv[0] << " <num_accounts>\n";
        return 1;
    }
    int num_accounts = std::stoi(argv[1]);
    Bank bank(num_accounts);
    std::cout << "Bank initialized with " << num_accounts << " accounts.\n";
    return 0;
}
