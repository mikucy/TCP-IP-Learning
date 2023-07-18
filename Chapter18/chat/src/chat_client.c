#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <pthread.h>

#define BUFFER_SIZE 1024
#define NAME_SIZE 20

char name[NAME_SIZE] = "[DEFAULT]";
char msg[BUFFER_SIZE];

void* send_message(void* arg) {
    int socket = *((int*)arg);
    char name_msg[NAME_SIZE + BUFFER_SIZE];
    while (1) {
        fgets(msg, BUFFER_SIZE, stdin);
        if (!strcmp(msg, "q\n") || !strcmp(msg, "Q\n")) {
            close(socket);
            exit(0);
        }
        sprintf(name_msg, "%s %s", name, msg);
        write(socket, name_msg, strlen(name_msg));
    }
}

void* receive_message(void* arg) {
    int socket = *((int*)arg), str_len = 0;
    char name_msg[NAME_SIZE + BUFFER_SIZE];
    while (1) {
        str_len = read(socket, name_msg, NAME_SIZE + BUFFER_SIZE - 1);
        if (str_len == -1) {
            return (void*)-1;
        }
        name_msg[str_len] = 0;
        fputs(name_msg, stdout);
    }
}

int main(int argc, char* argv[]) {
    if (argc != 4) {
        printf("Usage: %s [ip address] [port] [name]\n", argv[0]);
        exit(1);
    }
    int server_socket;
    struct sockaddr_in client_addr;

    sprintf(name, "[%s]", argv[3]);
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
    }

    pthread_t send_thread, receive_thread;
    pthread_create(&send_thread, NULL, send_message, (void*)&server_socket);
    pthread_create(&receive_thread, NULL, receive_message, (void*)&server_socket);
    void* thread_return;
    pthread_join(send_thread, &thread_return);
    pthread_join(receive_thread, &thread_return);

    close(server_socket);
}