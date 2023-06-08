#include <stdio.h>
#include <sys/types.h>          
#include <sys/socket.h>

int main() {
    int tcp_socket, udp_socket;
    tcp_socket = socket(PF_INET, SOCK_STREAM, 0);
    udp_socket = socket(PF_INET, SOCK_DGRAM, 0);
    printf("SOCK_STREAM: %d\n", SOCK_STREAM);
    printf("SOCK_DGRAM: %d\n", SOCK_DGRAM);
    int tcp_type, udp_type;
    socklen_t socklen;
    socklen = sizeof(tcp_type);
    getsockopt(tcp_socket, SOL_SOCKET, SO_TYPE, (void*)&tcp_type, &socklen);
    getsockopt(udp_socket, SOL_SOCKET, SO_TYPE, (void*)&udp_type, &socklen);
    printf("TCP type: %d, UDP type: %d\n", tcp_type, udp_type);
}