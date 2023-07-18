#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <errno.h>
#include <pthread.h>

#define BUFFER_SIZE 100 
#define MAX_CLNT 256
int client_count = 0;
int client_sockets[MAX_CLNT];
pthread_mutex_t mutex;

void* send_msg(char* msg, int str_len) {
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; ++i) {
        write(client_sockets[i], msg, str_len);
    }
    pthread_mutex_unlock(&mutex);
}

void* handle_client(void* arg) {
    int client_socket = *((int*)arg);
    int str_len = 0;
    char msg[BUFFER_SIZE];
    while ((str_len = read(client_socket, msg, sizeof(msg))) != 0) {
        send_msg(msg, str_len);
    }
    pthread_mutex_lock(&mutex);
    for (int i = 0; i < client_count; ++i) {
        if (client_socket == client_sockets[i]) {
            while (i++ < client_count - 1) {
                client_sockets[i] = client_sockets[i+1];
            }
            break;
        }
    }
    client_count--;
    pthread_mutex_unlock(&mutex);
    close(client_socket);
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }

    int server_socket, client_socket;
    struct sockaddr_in server_addr;
    struct sockaddr_in client_addr;
    int client_address_size;

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

    if (listen(server_socket, 5) == -1) {
        error_handling("listen() error");
    }

    pthread_mutex_init(&mutex, NULL);
    pthread_t t_id;

    while (1) {
        client_address_size = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &client_address_size);

        pthread_mutex_lock(&mutex);
        client_sockets[client_count++] = client_socket;
        pthread_mutex_unlock(&mutex);

        pthread_create(&t_id, NULL, handle_client, (void*)&client_socket);
        pthread_detach(t_id);
        printf("Connected client IP: %s \n", inet_ntoa(client_addr.sin_addr));
    }
    close(server_socket);
}