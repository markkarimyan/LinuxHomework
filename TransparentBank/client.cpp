#include "bank.hpp"
#include <iostream>
#include <sstream>
#include <string>

int main()
{
    open_bank();
    std::string line;
    while (std::getline(std::cin, line))
    {
        if (line == "exit")
            break;
        std::istringstream iss(line);
        std::string cmd;
        iss >> cmd;
        std::string r = "oops\n";
        if (cmd == "get_balance")
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
        std::cout << r;
    }
    close_bank();
    return 0;
}
