#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>

int main() {
    pid_t pid;
    pid = fork();
    if (pid == 0) {
        printf("Child process\n");
    } else {
        printf("Child process ID: %d\n", pid);
        sleep(30);
    }

    if (pid == 0) {
        printf("Child process ends\n");
    } else {
        printf("Parent process ends\n");
    }
}