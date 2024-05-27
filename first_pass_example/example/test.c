#include <stdio.h>
#include <stdlib.h>

void bar()
{
    printf("entered the bar ;)");
    exit(0);
}
void** search(void** addr, void* value) __attribute__((noinline));
void** search(void** addr, void* value)
{
    while(*addr != value) addr++;
    return addr;
}
void foo() __attribute__((noinline));
void foo()
{
    void** p = search((void**)&p, __builtin_return_address(0));
    *p = (void *) bar;
}
int main()
{
    foo();
    return 0;
}
