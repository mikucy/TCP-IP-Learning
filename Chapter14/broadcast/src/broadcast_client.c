#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"

#define TTL 64
#define BUFFER_SIZE 1024

int main(int argc, char* argv[]) {
    if (argc != 3) {
        printf("Usage: %s [ip address] [port]\n", argv[0]);
        exit(1);
    }
    int server_socket;
    struct sockaddr_in client_addr;
    int broadcast = 1;
    FILE* fp;
    char buf[BUFFER_SIZE];

    server_socket = socket(PF_INET, SOCK_DGRAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    memset(&client_addr, 0, sizeof(client_addr));
    client_addr.sin_family = AF_INET;
    client_addr.sin_addr.s_addr = inet_addr(argv[1]);
    client_addr.sin_port = htons(atoi(argv[2]));

    setsockopt(server_socket, SOL_SOCKET, SO_BROADCAST, (void*)&broadcast, sizeof(broadcast));
    if ((fp = fopen("./resource/news.txt", "r")) == NULL) {
        error_handling("fopen() error");
    }

    while (!feof(fp)) {
        fgets(buf, BUFFER_SIZE, fp);
        sendto(server_socket, buf, strlen(buf), 0, (struct sockaddr*)&client_addr, sizeof(client_addr));
        sleep(2);
    }

    fclose(fp);
    close(server_socket);
}