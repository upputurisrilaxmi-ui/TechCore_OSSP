#include <stdio.h>
#include <unistd.h>
#include <sys/wait.h>

int main()
{
    int num, fact = 1;
    printf("Enter a number: ");
    scanf("%d", &num);

    pid_t pid = fork();

    if (pid < 0)
    {
        printf("Failed to create child process\n");
        return 1;
    }
    else if (pid == 0)
    {
        // Child process
        for (int i = 1; i <= num; i++)
            fact = fact * i;

        printf("Child Process: Factorial of %d is %d\n", num, fact);
    }
    else
    {
        // Parent process
        wait(NULL);
        printf("Parent Process: Factorial of %d is %d\n", num, fact);
    }

    return 0;
}
