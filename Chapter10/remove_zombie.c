#include <stdio.h>
#include <unistd.h>
#include <signal.h>
#include <stdlib.h>
#include <sys/wait.h>

void read_child(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed child process %d\n", id);
        printf("Child send %d\n", WEXITSTATUS(status));
    }
}

int main() {
    struct sigaction act;
    act.sa_handler = read_child;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

    pid_t pid;
    pid = fork();
    if (pid == 0) {
        printf("Child process1\n");
        sleep(10);
        return 3;
    } else {
        printf("Child process ID: %d\n", pid);
        pid = fork();
        if (pid == 0) {
            printf("Child process2\n");
            sleep(10);
            exit(9);
        } else {
            printf("Child process ID: %d\n", pid);
            for (int i = 0; i < 5; ++i) {
                printf("Wait...\n");
                sleep(5);
            }
        }
    }
}