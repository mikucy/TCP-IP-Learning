#include <stdio.h>
#include <sys/wait.h>
#include <stdlib.h>
#include <unistd.h>

int main() {
    pid_t pid;
    int status;
    pid = fork();
    if (pid == 0) {
        sleep(15);
        return 24;
    } else {
        printf("Child process ID: %d\n", pid);
        while (!waitpid(-1, &status, WNOHANG)) {
            sleep(3);
            printf("Sleep 3s\n");
        }
        if (WIFEXITED(status)) {
            printf("Child send %d\n", WEXITSTATUS(status));
        }
    }
}