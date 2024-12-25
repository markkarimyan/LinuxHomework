#include <iostream>
#include <pthread.h>
#include <vector>
#include <cstdlib>

class BankAccount {
private:
    int balance;
    pthread_mutex_t mutex;

public:
    BankAccount(int initial_balance = 0) : balance(initial_balance) {
        pthread_mutex_init(&mutex, nullptr);
    }

    ~BankAccount() {
        pthread_mutex_destroy(&mutex);
    }

    void debit(int amount) {
        pthread_mutex_lock(&mutex);
        balance -= amount;
        pthread_mutex_unlock(&mutex);
    }

    void credit(int amount) {
        pthread_mutex_lock(&mutex);
        balance += amount;
        pthread_mutex_unlock(&mutex);
    }

    int getBalance() {
        pthread_mutex_lock(&mutex);
        int current_balance = balance;
        pthread_mutex_unlock(&mutex);
        return current_balance;
    }
};

struct Operation {
    BankAccount* account;
    int amount;
    char operation;
};

void* performOperation(void* arg) {
    Operation* op = (Operation*)arg;
    
    if (op->operation == 'd') {
        op->account->debit(op->amount);
    } else if (op->operation == 'c') {
        op->account->credit(op->amount);
    }

    return nullptr;
}

int main() {
    int numThreads;
    std::cout << "Enter the number of operations: ";
    std::cin >> numThreads;

    BankAccount account(1000);

    std::vector<pthread_t> threads(numThreads);
    std::vector<Operation> operations(numThreads);

    for (int i = 0; i < numThreads; ++i) {
        int amount;
        char op;
        std::cout << "Enter operation #" << i + 1 << " (amount operation_type [d = debit, c = credit]): ";
        std::cin >> amount >> op;
        
        operations[i].account = &account;
        operations[i].amount = amount;
        operations[i].operation = op;

        pthread_create(&threads[i], nullptr, performOperation, &operations[i]);
    }

    for (int i = 0; i < numThreads; ++i) {
        pthread_join(threads[i], nullptr);
    }

    std::cout << "Final balance: " << account.getBalance() << std::endl;
    return 0;
}
