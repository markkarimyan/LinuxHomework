#include <sys/fcntl.h>
#include <sys/mman.h>
#include <iostream>

int main() {
    //int shm_unlink(const char *bank_shm);
    shm_unlink("/bank_shm");
    return 0;
}