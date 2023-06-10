#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int fd[2];
    pipe(fd);
    char str1[] = "Child message";
    char str2[] = "Parent message";
    char buf[1024];
    pid_t pid = fork();
    if (pid == 0) {
        write(fd[1], str1, sizeof(str1));
        sleep(2);
        int str_len = read(fd[0], buf, 1024);
        printf("Child process: %s\n", buf);
    } else {
        int str_len = read(fd[0], buf, 1024);
        printf("Parent process: %s\n", buf);
        write(fd[1], str2, sizeof(str2));
        sleep(3);
    }
}