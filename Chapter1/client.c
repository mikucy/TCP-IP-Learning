#include <stdio.h> 
#include <stdlib.h>
#include <string.h> 
#include <unistd.h>
#include <arpa/inet.h> 
#include <sys/socket.h>

void error_handling(char* message);

int main(int argc, char *argv[]) {
    int server_socket;
    struct sockaddr_in server_address;
    char message[30];
    int str_len;

    if (argc != 3) {
        printf("Usage: %s <IP> <port>\n", argv[0]);
        exit(1);
    }

    server_socket = socket(PF_INET, SOCK_STREAM, 0);
    if (server_socket == -1) {
        error_handling("socket() error");
    }

    memset(&server_address, 0, sizeof(server_socket));
    server_address.sin_family = AF_INET;
    server_address.sin_addr.s_addr = inet_addr(argv[1]);
    server_address.sin_port = htons(atoi(argv[2]));

    if (connect(server_socket, (struct sockaddr*)&server_address, sizeof(server_address)) == -1) {
        error_handling("connect() error");
    }
    
    if (read(server_socket, message, sizeof(message)) == -1) {
        error_handling("read() error");
    }
    printf("Message from server: %s \n", message);
    close(server_socket);
    return 0;
}