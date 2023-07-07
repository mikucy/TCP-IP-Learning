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

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    int server_socket;
    int str_len;
    struct sockaddr_in server_addr;
    char buf[BUFFER_SIZE];
    struct ip_mreq join_address;

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

    join_address.imr_multiaddr.s_addr = inet_addr(argv[1]);
    join_address.imr_interface.s_addr = htonl(INADDR_ANY);

    setsockopt(server_socket, IPPROTO_IP, IP_ADD_MEMBERSHIP, (void*)&join_address, sizeof(join_address));
    while (1) {
        str_len = recvfrom(server_socket, buf, BUFFER_SIZE - 1, 0, NULL, 0);
        if (str_len < 0) {
            break;
        }
        buf[str_len] = 0;
        fputs(buf, stdout);
        fflush(stdout);
    }
    close(server_socket);
}