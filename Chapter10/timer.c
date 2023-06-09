#include <stdio.h>
#include <signal.h>
#include <unistd.h>
#include <stdlib.h>
#include <string.h>

void timeout(int sig) {
    printf("Timeout\n");
    alarm(1);
}

void keycontrol(int sig) {
    printf("Are you sure to quit the program? (Y to quit)\n");
    char buf;
    scanf("%c", &buf);
    if (buf == 'Y' || buf == 'y') {
        printf("Exit\n");
        exit(0);
    } else {
        printf("Continue running\n");
    }
}

int main() {
    struct sigaction act;
    act.sa_handler = timeout;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGALRM, &act, 0);
    struct sigaction act1;
    act1.sa_handler = keycontrol;
    sigemptyset(&act1.sa_mask);
    sigaddset(&act1.sa_mask, SIGALRM);
    act1.sa_flags = 0;
    sigaction(SIGINT, &act1, 0);

    alarm(1);
    while (1);
}