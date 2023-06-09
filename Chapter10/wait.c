#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>
#include <sys/wait.h>

int main() {
    pid_t pid;
    int state;
    pid = fork();
    if (pid == 0) {
        return 3;
    } else {
        printf("Child process PID: %d\n", pid);
        pid = fork();
        if (pid == 0) {
            exit(9);
        } else {
            printf("Child process PID: %d\n", pid);
            wait(&state);
            if (WIFEXITED(state)) {
                printf("Child send 1: %d\n", WEXITSTATUS(state));
            }

            wait(&state);
            if (WIFEXITED(state)) {
                printf("Child send 2: %d\n", WEXITSTATUS(state));
            }
            sleep(30);
        }
    }
}