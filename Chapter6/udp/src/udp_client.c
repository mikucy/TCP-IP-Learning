#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s [ip address] [port]\n", argv[0]);
        exit(1);
    }
    int server_socket;
    struct sockaddr_in client_addr, from_addr;

    server_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(argv[1]);
    client_addr.sin_port = htons(atoi(argv[2]));

    char buf[BUFFER_SIZE];
    socklen_t socklen;
    int str_len;
    while (1) {
        printf("Input message(Q to quit): \n");
        scanf("%s", buf);
        if (!strcmp(buf, "q") || !strcmp(buf, "Q")) {
            break;
        }
        sendto(server_socket, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        socklen = sizeof(from_addr);
        str_len = recvfrom(server_socket, buf, BUFFER_SIZE, 0, (struct sockaddr*)&from_addr, &socklen);
        buf[str_len] = '\0';
        printf("Message from server: %s\n", buf);
    }

    close(server_socket);
}