#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include <sys/types.h>

int main() {
    int fd1[2], fd2[2];

    pipe(fd1);
    pipe(fd2);
    char str1[] = "Child message";
    char str2[] = "Parent message";
    char buf[1024];
    pid_t pid = fork();
    if (pid == 0) {
        write(fd1[1], str1, sizeof(str1));
        int str_len = read(fd2[0], buf, 1024);
        printf("Child process: %s\n", buf);
    } else {
        int str_len = read(fd1[0], buf, 1024);
        printf("Parent process: %s\n", buf);
        write(fd2[1], str2, sizeof(str2));
    }
}