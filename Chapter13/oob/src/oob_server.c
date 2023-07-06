#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <sys/select.h>
#include <fcntl.h>
#include <signal.h>

#define BUFFER_SIZE 1024

int server_socket, client_socket;

void urg_handler(int sig) {
    int str_len;
    char buf[BUFFER_SIZE];
    str_len = recv(client_socket, buf, BUFFER_SIZE, MSG_OOB);
    buf[str_len] = 0;
    printf("Urgent message: %s\n", buf);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    socklen_t address_size;
    int str_len;
    char buf[BUFFER_SIZE];
    struct timeval timeout;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_socket, 1) == -1) {
        error_handling("listen() error");
    }

    address_size = sizeof(server_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&server_addr, &address_size);

    fcntl(client_socket, F_SETOWN, getpid());
    struct sigaction act;
    act.sa_handler = urg_handler;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    int state = sigaction(SIGURG, &act, 0);
    while ((str_len = recv(client_socket, buf, BUFFER_SIZE, 0)) != 0) {
        if (str_len == -1) {
            continue;
        }
        buf[str_len] = 0;
        printf("%s\n", buf);
    }
    close(client_socket);
    close(server_socket);
}