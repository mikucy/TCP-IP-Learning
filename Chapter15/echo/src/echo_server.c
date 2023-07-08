#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    FILE* writefp;
    FILE* readfp;

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
    char buf[1024];
    int strlen;
    for (int i = 0; i < 5; ++i) {
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
        if (client_socket == -1) {
            error_handling("accept() error");
        }
        writefp = fdopen(client_socket, "w");
        readfp = fdopen(client_socket, "r");
        while (!feof(readfp)) {
            fgets(buf, 1024, readfp);
            fputs(buf, writefp);
            fflush(writefp);
        }
        fclose(writefp);
        fclose(readfp);
    }
    close(server_socket);
}