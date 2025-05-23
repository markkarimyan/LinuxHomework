#ifndef BANK_HPP
#define BANK_HPP

#include <string>

int init_bank(int N, int max_balance);
int deinit_bank();
int open_bank();
int close_bank();
std::string get_balance(int A);
std::string get_min_balance(int A);
std::string get_max_balance(int A);
std::string freeze_account(int A);
std::string unfreeze_account(int A);
std::string transfer_amount(int A, int B, int X);
std::string deposit_all(int X);
std::string withdraw_all(int X);
std::string set_min_balance(int A, int X);
std::string set_max_balance(int A, int X);

#endif
