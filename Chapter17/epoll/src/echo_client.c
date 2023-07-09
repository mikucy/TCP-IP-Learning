#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

#define BUFFER_SIZE 1024

void read_routine(int socket, char* buf) {
    while (1) {
        int str_len = read(socket, buf, BUFFER_SIZE);
        if (str_len == 0) {
            return;
        }
        buf[str_len] = '\0';
        printf("Message from server: %s\n", buf);
    }
}

void write_routine(int socket, char* buf) {
    while (1) {
        scanf("%s", buf);
        if (!strcmp(buf, "q") || !strcmp(buf, "Q")) {
            shutdown(socket, SHUT_WR);
            return;
        }
        write(socket, buf, strlen(buf));
    }
}

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s [ip address] [port]\n", argv[0]);
        exit(1);
    }
    int server_socket;
    struct sockaddr_in client_addr;

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

    pid_t pid = fork();
    char buf[BUFFER_SIZE];
    if (pid == 0) {
        read_routine(server_socket, buf);
    } else {
        write_routine(server_socket, buf);
    }

    close(server_socket);
}