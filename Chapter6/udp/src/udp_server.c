#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    int server_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;

    server_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    memset(&server_addr, 0, sizeof(server_addr));
    server_addr.sin_family = AF_INET;
    server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
    server_addr.sin_port = htons(atoi(argv[1]));

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    socklen_t socklen;
    int str_len;
    char buf[BUFFER_SIZE];
    while (1) {
        socklen = sizeof(client_addr);
        str_len = recvfrom(server_socket, buf, BUFFER_SIZE, 0, (struct sockaddr*)&client_addr, &socklen);
        sendto(server_socket, buf, str_len, 0, (struct sockaddr*)&client_addr, socklen);
    }

    close(server_socket);
}