#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

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
    uint8_t count;
    fputs("Operand count: ", stdout);
    scanf("%hhd", &count);
    write(server_socket, &count, sizeof(count));

    for (uint8_t i = 0; i < count; ++i) {
        int num;
        printf("Operand %d: ", i+1);
        scanf("%d", &num);
        write(server_socket, &num, sizeof(num));
    }

    char operator;
    fputs("Operator: ", stdout);
    scanf(" %c", &operator);
    write(server_socket, &operator, sizeof(operator));

    int res;
    if ((len = read(server_socket, &res, sizeof(res))) == 0) {
        error_handling("read() error");
    }
    printf("Operation result: %d\n", res);
    
    close(server_socket);
}