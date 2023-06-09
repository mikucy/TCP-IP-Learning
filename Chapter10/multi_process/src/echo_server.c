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
#include <sys/wait.h>
#include <signal.h>

#define BUFFER_SIZE 1024

void read_child(int sig) {
    int status;
    pid_t id = waitpid(-1, &status, WNOHANG);
    if (WIFEXITED(status)) {
        printf("Removed child process %d\n", id);
        printf("Child send %d\n", WEXITSTATUS(status));
    }
}

int main(int argc, char* argv[]) {
    if (argc != 2) {
        printf("Usage: %s [port]\n", argv[0]);
        exit(1);
    }
    struct sigaction act;
    act.sa_handler = read_child;
    sigemptyset(&act.sa_mask);
    act.sa_flags = 0;
    sigaction(SIGCHLD, &act, 0);

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

    pid_t pid;
    int str_len;
    char buf[BUFFER_SIZE];
    while (1) {
        socklen_t addrlen = sizeof(client_addr);
        client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &addrlen);
        pid = fork();
        if (pid == -1) {
            continue;
        }
        if (pid == 0) {
            close(server_socket);
            while ((str_len = read(client_socket, buf, BUFFER_SIZE)) != 0) {
                write(client_socket, buf, str_len);
            }
            close(client_socket);
            printf("Client disconnected...\n");
            return 0;
        } else {
            close(client_socket);
        }
    }
    close(server_socket);
}