#include "bank.hpp"
#include <cstdlib>

int main(int argc, char **argv)
{
    if (argc != 3)
        return 1;
    return init_bank(std::atoi(argv[1]), std::atoi(argv[2]));
}
