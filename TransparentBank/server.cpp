#include "bank.hpp"
#include <pthread.h>
#include <netinet/in.h>
#include <unistd.h>
#include <sstream>
#include <string>
#include <iostream>

static int processed = 0;
static pthread_mutex_t pm = PTHREAD_MUTEX_INITIALIZER;

void *handler(void *p)
{
    int fd = *(int *)p;
    delete (int *)p;
    char buf[256];
    while (true)
    {
        int n = read(fd, buf, 255);
        if (n <= 0)
            break;
        buf[n] = 0;
        std::string line(buf);
        if (line.back() == '\n')
            line.pop_back();
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        std::string r = "oops\n";
        if (cmd == "shutdown")
        {
            r = "done\n";
            write(fd, r.c_str(), r.size());
            exit(0);
        }
        else if (cmd == "get_balance")
        {
            int A;
            iss >> A;
            r = get_balance(A);
        }
        else if (cmd == "get_min")
        {
            int A;
            iss >> A;
            r = get_min_balance(A);
        }
        else if (cmd == "get_max")
        {
            int A;
            iss >> A;
            r = get_max_balance(A);
        }
        else if (cmd == "freeze")
        {
            int A;
            iss >> A;
            r = freeze_account(A);
        }
        else if (cmd == "unfreeze")
        {
            int A;
            iss >> A;
            r = unfreeze_account(A);
        }
        else if (cmd == "transfer")
        {
            int A, B, X;
            iss >> A >> B >> X;
            r = transfer_amount(A, B, X);
        }
        else if (cmd == "deposit_all")
        {
            int X;
            iss >> X;
            r = deposit_all(X);
        }
        else if (cmd == "withdraw_all")
        {
            int X;
            iss >> X;
            r = withdraw_all(X);
        }
        else if (cmd == "set_min")
        {
            int A, X;
            iss >> A >> X;
            r = set_min_balance(A, X);
        }
        else if (cmd == "set_max")
        {
            int A, X;
            iss >> A >> X;
            r = set_max_balance(A, X);
        }
        write(fd, r.c_str(), r.size());
        pthread_mutex_lock(&pm);
        processed++;
        if (processed % 5 == 0)
            std::cout << "Processed " << processed << " requests\n";
        pthread_mutex_unlock(&pm);
    }
    close(fd);
    return 0;
}

int main()
{
    open_bank();
    int s = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in a{};
    a.sin_family = AF_INET;
    a.sin_addr.s_addr = INADDR_ANY;
    a.sin_port = htons(12345);
    bind(s, (sockaddr *)&a, sizeof(a));
    listen(s, 5);
    while (true)
    {
        int *c = new int;
        *c = accept(s, 0, 0);
        pthread_t t;
        pthread_create(&t, 0, handler, c);
        pthread_detach(t);
    }
    return 0;
}
