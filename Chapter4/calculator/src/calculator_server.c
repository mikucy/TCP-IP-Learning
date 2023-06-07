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
    uint8_t count;
    int len;
    if ((len = read(client_socket, &count, sizeof(count))) == 0) {
        error_handling("read count error");
    }
    printf("Read count: %hhd\n", count);
    int nums[count];
    for (int i = 0; i < count; ++i) {
        if ((len = read(client_socket, &nums[i], sizeof(int))) == 0) {
            error_handling("read number error");
        }
        printf("Read number: %d\n", nums[i]);
    }
    char operator;
    if ((len = read(client_socket, &operator, sizeof(char))) == 0) {
        error_handling("read operator error");
    }
    switch (operator) {
        case '*':
            for (int i = 1; i < count; ++i) {
                nums[0] *= nums[i];
            }
            break;
        case '-':
            for (int i = 1; i < count; ++i) {
                nums[0] -= nums[i];
            }
            break;
        case '+':
            for (int i = 1; i < count; ++i) {
                nums[0] += nums[i];
            }
            break;
        default:
            error_handling("operator invalid");
    }
    printf("Got result: %d\n", nums[0]);
    write(client_socket, &nums[0], sizeof(nums[0]));

    close(client_socket);
    close(server_socket);
}