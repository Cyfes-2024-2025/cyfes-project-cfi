#include <fcntl.h>
#include <stddef.h>
#include <sys/mman.h>

volatile void* __global_ptrauth_device_base;

__attribute__((constructor))
void __setup_ptrauth_globals() {
    int fd = open("/dev/ptrauth", O_RDWR);
    __global_ptrauth_device_base = mmap(NULL, 0x1000, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
}
