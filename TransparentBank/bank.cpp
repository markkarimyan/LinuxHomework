#include "bank.hpp"
#include <sys/mman.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <pthread.h>
#include <sstream>
#include <sys/types.h>
#include <sys/stat.h>

struct Account
{
    int balance, min_balance, max_balance;
    bool frozen;
};
struct Bank
{
    pthread_mutex_t mutex;
    int num_accounts;
    Account accounts[];
};

static Bank *bank = nullptr;
static size_t shm_size = 0;

int init_bank(int N, int max_balance)
{
    int fd = shm_open("/bank_shm", O_CREAT | O_RDWR, 0666);
    shm_size = sizeof(Bank) + N * sizeof(Account);
    ftruncate(fd, shm_size);
    bank = (Bank *)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    pthread_mutex_init(&bank->mutex, 0);
    bank->num_accounts = N;
    for (int i = 0; i < N; i++)
    {
        bank->accounts[i].balance = 0;
        bank->accounts[i].min_balance = 0;
        bank->accounts[i].max_balance = max_balance;
        bank->accounts[i].frozen = false;
    }
    return 0;
}

int deinit_bank()
{
    shm_unlink("/bank_shm");
    return 0;
}

int open_bank()
{
    int fd = shm_open("/bank_shm", O_RDWR, 0666);
    struct stat st;
    fstat(fd, &st);
    shm_size = st.st_size;
    bank = (Bank *)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
    return 0;
}

int close_bank()
{
    munmap(bank, shm_size);
    return 0;
}

std::string get_balance(int A)
{
    pthread_mutex_lock(&bank->mutex);
    std::ostringstream o;
    if (A < 0 || A >= bank->num_accounts)
        o << "oops\n";
    else
        o << bank->accounts[A].balance << "\n";
    pthread_mutex_unlock(&bank->mutex);
    return o.str();
}

std::string get_min_balance(int A)
{
    pthread_mutex_lock(&bank->mutex);
    std::ostringstream o;
    if (A < 0 || A >= bank->num_accounts)
        o << "oops\n";
    else
        o << bank->accounts[A].min_balance << "\n";
    pthread_mutex_unlock(&bank->mutex);
    return o.str();
}

std::string get_max_balance(int A)
{
    pthread_mutex_lock(&bank->mutex);
    std::ostringstream o;
    if (A < 0 || A >= bank->num_accounts)
        o << "oops\n";
    else
        o << bank->accounts[A].max_balance << "\n";
    pthread_mutex_unlock(&bank->mutex);
    return o.str();
}

std::string freeze_account(int A)
{
    pthread_mutex_lock(&bank->mutex);
    std::string r = (A < 0 || A >= bank->num_accounts ? "oops\n" : (bank->accounts[A].frozen = true, "done\n"));
    pthread_mutex_unlock(&bank->mutex);
    return r;
}

std::string unfreeze_account(int A)
{
    pthread_mutex_lock(&bank->mutex);
    std::string r = (A < 0 || A >= bank->num_accounts ? "oops\n" : (bank->accounts[A].frozen = false, "done\n"));
    pthread_mutex_unlock(&bank->mutex);
    return r;
}

std::string transfer_amount(int A, int B, int X)
{
    pthread_mutex_lock(&bank->mutex);
    bool ok = !(A < 0 || A >= bank->num_accounts || B < 0 || B >= bank->num_accounts || X <= 0 || bank->accounts[A].frozen || bank->accounts[B].frozen || bank->accounts[A].balance - X < bank->accounts[A].min_balance || bank->accounts[B].balance + X > bank->accounts[B].max_balance);
    if (ok)
    {
        bank->accounts[A].balance -= X;
        bank->accounts[B].balance += X;
    }
    pthread_mutex_unlock(&bank->mutex);
    return ok ? "done\n" : "oops\n";
}

std::string deposit_all(int X)
{
    pthread_mutex_lock(&bank->mutex);
    bool ok = true;
    for (int i = 0; i < bank->num_accounts; i++)
    {
        if (bank->accounts[i].frozen || bank->accounts[i].balance + X > bank->accounts[i].max_balance)
        {
            ok = false;
            break;
        }
    }
    if (ok)
        for (int i = 0; i < bank->num_accounts; i++)
            bank->accounts[i].balance += X;
    pthread_mutex_unlock(&bank->mutex);
    return ok ? "done\n" : "oops\n";
}

std::string withdraw_all(int X)
{
    pthread_mutex_lock(&bank->mutex);
    bool ok = true;
    for (int i = 0; i < bank->num_accounts; i++)
    {
        if (bank->accounts[i].frozen || bank->accounts[i].balance - X < bank->accounts[i].min_balance)
        {
            ok = false;
            break;
        }
    }
    if (ok)
        for (int i = 0; i < bank->num_accounts; i++)
            bank->accounts[i].balance -= X;
    pthread_mutex_unlock(&bank->mutex);
    return ok ? "done\n" : "oops\n";
}

std::string set_min_balance(int A, int X)
{
    pthread_mutex_lock(&bank->mutex);
    bool ok = !(A < 0 || A >= bank->num_accounts || X > bank->accounts[A].max_balance || bank->accounts[A].balance < X);
    if (ok)
        bank->accounts[A].min_balance = X;
    pthread_mutex_unlock(&bank->mutex);
    return ok ? "done\n" : "oops\n";
}

std::string set_max_balance(int A, int X)
{
    pthread_mutex_lock(&bank->mutex);
    bool ok = !(A < 0 || A >= bank->num_accounts || X < bank->accounts[A].min_balance || bank->accounts[A].balance > X);
    if (ok)
        bank->accounts[A].max_balance = X;
    pthread_mutex_unlock(&bank->mutex);
    return ok ? "done\n" : "oops\n";
}
