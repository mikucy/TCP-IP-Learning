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

    int len;
    char filepath[BUFFER_SIZE];
    printf("Please enter the file path: ");
    scanf("%s", filepath);
    write(server_socket, filepath, strlen(filepath));
    if ((len = read(server_socket, filepath, BUFFER_SIZE)) == 0) {
        error_handling("read() error");
    }
    printf("%s\n", filepath);
    write(server_socket, "Thank you", 10);

    close(server_socket);
}