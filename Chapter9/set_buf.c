#include <stdio.h>
#include <stdlib.h>
#include <sys/socket.h>

int main() {
    int tcp_socket;
    int send_buf = 3 * 1024, receive_buf = 3 * 1024;
    socklen_t socklen;
    tcp_socket = socket(PF_INET, SOCK_STREAM, 0);

    socklen = sizeof(send_buf);
    setsockopt(tcp_socket, SOL_SOCKET, SO_SNDBUF, (void*)&send_buf, sizeof(send_buf));
    setsockopt(tcp_socket, SOL_SOCKET, SO_RCVBUF, (void*)&receive_buf, sizeof(receive_buf));

    getsockopt(tcp_socket, SOL_SOCKET, SO_SNDBUF, (void*)&send_buf, &socklen);
    getsockopt(tcp_socket, SOL_SOCKET, SO_RCVBUF, (void*)&receive_buf, &socklen);

    printf("Input buffer size: %d\n", receive_buf);
    printf("Output buffer size: %d\n", send_buf);
}