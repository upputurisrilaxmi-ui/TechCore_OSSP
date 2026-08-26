#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main()
{
    pid_t pid;
    char pdf[100];

    printf("Enter PDF file path:Cases ");
    scanf("%s", pdf);

    pid = fork();

    if (pid < 0)
    {
        printf("Fork Failed\n");
        exit(1);
    }

    if (pid == 0)
    {
        // Child Process
        printf("Child Process: Opening PDF...\n");

        execlp("xdg-open", "xdg-open", pdf, NULL);

        // Executes only if exec fails
        perror("exec failed");
        exit(1);
    }
    else
    {
        // Parent Process
        printf("Parent Process: Browser is still running.\n");

        wait(NULL);

        printf("PDF viewer process completed.\n");
    }

    return 0;
}
