#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include "common.h"
#include <sys/select.h>
#include <sys/epoll.h>

#define BUFFER_SIZE 1024
#define EPOLL_SIZE 50

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

    struct epoll_event* ep_events;
    struct epoll_event event;
    int epfd, event_cnt, str_len;
    char buf[BUFFER_SIZE];

    epfd = epoll_create(EPOLL_SIZE);
    ep_events = malloc(sizeof(struct epoll_event) * EPOLL_SIZE);

    event.events = EPOLLIN;
    event.data.fd = server_socket;
    epoll_ctl(epfd, EPOLL_CTL_ADD, server_socket, &event);
    socklen_t address_size;
    while (1) {
        event_cnt = epoll_wait(epfd, ep_events, EPOLL_SIZE, -1);
        if (event_cnt == -1) {
            puts("epoll_wait() error");
            break;
        }
        for (int i = 0; i < event_cnt; ++i) {
            if (ep_events[i].data.fd == server_socket) {
                // Connectiont request.
                address_size = sizeof(client_addr);
                client_socket = accept(server_socket, (struct sockaddr*)&client_addr, &address_size);
                event.events = EPOLLIN;
                event.data.fd = client_socket;
                epoll_ctl(epfd, EPOLL_CTL_ADD, client_socket, &event);
                printf("Connected client: %d\n", client_socket);
            } else {
                str_len = read(ep_events[i].data.fd, buf, BUFFER_SIZE);
                if (str_len == 0) {
                    // Close request.
                    epoll_ctl(epfd, EPOLL_CTL_DEL, ep_events[i].data.fd, NULL);
                    close(ep_events[i].data.fd);
                    printf("Closed client: %d\n", ep_events[i].data.fd);
                } else {
                    // Echo.
                    write(ep_events[i].data.fd, buf, str_len);
                }
            }
        }
    }
    close(server_socket);
    close(epfd);
}