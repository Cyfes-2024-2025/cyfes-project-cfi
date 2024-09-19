#include <signal.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/signal.h>

void rop1(void)
{
    printf("Inside the rop1 function!\n");
    printf("Running ls\n");
    system("ls");
    printf("Let's look at some files..\n");
    printf("\n/etc/shadow\n");
    system("cat /etc/shadow");
    printf("\n/etc/passwd\n");
    system("cat /etc/passwd");
    exit(0);
}

void sigusr1Handler (int Sig) {
    printf("Received SIGUSR1\n");
    exit(0);
}

void vulnerable(char* String)
{
    char Buffer[100];
    strcpy(Buffer, String);
}

int main(int argc, char** argv)
{
    signal(SIGUSR1, sigusr1Handler);
    if (argc != 2) {
        printf("Usage: %s <string>\n", argv[0]);
        return 1;
    }
    printf("Calling the vulnerable function...\n");
    vulnerable(argv[1]);
    return 0;
}
