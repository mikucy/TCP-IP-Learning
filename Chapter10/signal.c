#include <stdio.h>
#include <signal.h>
#include <unistd.h>

void timeout(int sig) {
    if (sig == SIGALRM) {
        printf("Time out\n");
    }
    alarm(2);
}

void keycontrol(int sig) {
    if (sig == SIGINT) {
        printf("CTRL+C pressed\n");
    }
}

int main() {
    signal(SIGALRM, timeout);
    signal(SIGINT, keycontrol);
    alarm(2);

    for (int i = 0; i < 3; ++i) {
        printf("Wait...\n");
        sleep(100);
    }
}