#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <sys/select.h>

#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    int server_socket, client_socket;
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

    fd_set reads, cpy_reads;
    socklen_t address_size;
    int fd_max, str_len, fd_num;
    char buf[BUFFER_SIZE];
    struct timeval timeout;

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_socket, 1) == -1) {
        error_handling("listen() error");
    }

    FD_ZERO(&reads);
    FD_SET(server_socket, &reads);
    fd_max = server_socket;
    while (1) {
        cpy_reads = reads;
        timeout.tv_sec = 5;
        timeout.tv_usec = 5000;

        if ((fd_num = select(fd_max + 1, &cpy_reads, NULL, NULL, &timeout)) == -1) {
            break;
        }
        if (fd_num == 0) {
            continue;
        }
        for (int i = 0; i < fd_max + 1; ++i) {
            if (FD_ISSET(i, &cpy_reads)) {
                if (i == server_socket) {
                    // Connectiont request.
                    address_size = sizeof(client_addr);
                    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &address_size);
                    FD_SET(client_socket, &reads);
                    if (fd_max < client_socket) {
                        fd_max = client_socket;
                    }
                    printf("Connected client: %d\n", client_socket);
                } else {
                    str_len = read(i, buf, BUFFER_SIZE);
                    if (str_len == 0) {
                        // Close request.
                        FD_CLR(i, &reads);
                        close(i);
                        printf("Closed client: %d\n", i);
                    } else {
                        // Echo.
                        write(i, buf, str_len);
                    }
                }
            }
        }
    }
    close(server_socket);
}