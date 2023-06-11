#include <stdio.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/select.h>

int main() {
    fd_set reads, temps;
    int result, str_len;
    char buf[1024];
    struct timeval timeout;
    FD_ZERO(&reads);
    FD_SET(0, &reads);

    while (1) {
        temps = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 0;
        result = select(1, &temps, NULL, NULL, &timeout);
        if (result == -1) {
            printf("select() error\n");
            break;
        } else if (result == 0) {
            printf("Timeout\n");
        } else {
            if (FD_ISSET(0, &temps)) {
                str_len = read(0, buf, 1024);
                buf[str_len] = 0;
                printf("Message from stdin: %s", buf);
            }
        }
    }
}