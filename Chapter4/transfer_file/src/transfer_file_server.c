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

    if (bind(server_socket, (struct sockaddr*)&server_addr, sizeof(server_addr)) == -1) {
        error_handling("bind() error");
    }

    if (listen(server_socket, 1) == -1) {
        error_handling("listen() error");
    }

    socklen_t addrlen = sizeof(client_addr);
    client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
    if (client_socket == -1) {
        error_handling("accept() error");
    }
    char filepath[BUFFER_SIZE];
    int len;
    if ((len = read(client_socket, filepath, BUFFER_SIZE)) == 0) {
        error_handling("read() error for socket");
    }
    printf("Got filepath: %s\n", filepath);

    if (access(filepath, F_OK) == 0) {
        printf("File exists\n");
        int fd;
        if ((fd = open(filepath, O_RDONLY)) == -1) {
            error_handling("open() error");
        }
        char buf[BUFFER_SIZE];
        if ((len = read(fd, buf, BUFFER_SIZE)) == 0) {
            error_handling("read() error for file");
        }
        write(client_socket, buf, strlen(buf));
    } else {
        printf("File not exists\n");
    }

    close(client_socket);
    close(server_socket);
}