#include <stdio.h>
#include <unistd.h>
#include <sys/types.h>
#include <string.h>

int main() {
    int fd[2];
    pipe(fd);
    pid_t pid = fork();
    if (pid == 0) {
        char* buf = "This is pipe";
        write(fd[1], buf, strlen(buf));
    } else {
        char buf[1024];
        int str_len = read(fd[0], buf, 1024);
        buf[str_len] = '\0';
        printf("%s\n", buf);
    }
}