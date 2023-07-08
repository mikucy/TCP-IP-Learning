#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s [ip address] [port]\n", argv[0]);
        exit(1);
    }
    int server_socket;
    struct sockaddr_in client_addr;
    FILE* writefp;
    FILE* readfp;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(argv[1]);
    client_addr.sin_port = htons(atoi(argv[2]));

    if (connect(server_socket, (struct sockaddr*)&client_addr, sizeof(client_addr)) == -1) {
        error_handling("connect() error");
    } else {
        puts("Connected");
    }

    char buf[1024];
    int str_len;

    writefp = fdopen(server_socket, "w");
    readfp = fdopen(server_socket, "r");
    while (1) {
        fputs("Input message, Q to quit\n", stdout);
        fgets(buf, 1024, stdin);

        if (!strcmp(buf, "q\n") || !strcmp(buf, "Q\n")) {
            break;
        }
        fputs(buf, writefp);
        fflush(writefp);
        fgets(buf, 1024, readfp);
        printf("Message from server: %s\n", buf);
    }
    close(server_socket);
}