#include "colorprint.hpp"
#include <iostream>
#include <vector>
#include <string>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <cstdlib>

int main(int ac, char **av)
{
    if (ac != 3)
        return 1;
    int sock = socket(AF_INET, SOCK_STREAM, 0);
    sockaddr_in sr{};
    sr.sin_family = AF_INET;
    sr.sin_port = htons(std::atoi(av[2]));
    inet_pton(AF_INET, av[1], &sr.sin_addr);
    connect(sock, (sockaddr *)&sr, sizeof(sr));
    std::vector<std::string> ok = {"done"}, bad = {"oops"};
    Painter p(std::cout, ok, bad);
    std::string l;
    while (std::getline(std::cin, l))
    {
        if (l == "exit")
            break;
        l += "\n";
        write(sock, l.c_str(), l.size());
        char b[256];
        int n = read(sock, b, 255);
        if (n <= 0)
            break;
        b[n] = 0;
        p.printColoredLine(std::string(b));
    }
    close(sock);
    return 0;
}
