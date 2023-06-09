#include <stdio.h>
#include <sys/types.h>
#include <unistd.h>

int gval = 10;
int main()
{
    pid_t pid;
    int lval = 5;
    pid = fork();
    if (pid == 0) {
        gval += 2;
        lval += 2;
    } else {
        gval -= 2;
        lval -= 2;
    }

    if (pid == 0) {
        printf("Child process: [%d, %d]\n", gval, lval);
    } else {
        printf("Parent process: [%d, %d]\n", gval, lval);
    }
}