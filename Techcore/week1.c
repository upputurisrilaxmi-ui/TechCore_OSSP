#include <stdio.h>
#include <unistd.h>

int main()
{
    int pid;

    printf("Start of parent\n");

    pid = fork();

    if (pid == 0)
    {
        sleep(10);
    }

    printf("End\n");

    return 0;
}
