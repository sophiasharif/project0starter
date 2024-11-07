#include "server_socket.h"
#include <stdexcept>
#include <unistd.h>
#include <iostream>

using namespace std;

ServerSocket::ServerSocket(int port) : Socket(port), client_connected(0)
{
    bind_socket();
}

void ServerSocket::bind_socket()
{
    int did_bind = bind(sockfd, (struct sockaddr *)&servaddr,
                        sizeof(servaddr));
    if (did_bind < 0)
    {
        throw runtime_error("Failed to bind socket");
    }
}

int ServerSocket::read_from_socket(uint8_t *buf, size_t length)
{

    socklen_t address_size = sizeof(clientaddr);
    int bytes_recvd = recvfrom(sockfd, buf, length,
                               0, (struct sockaddr *)&clientaddr,
                               &address_size);
    if (bytes_recvd > 0)
        client_connected = 1;
    return bytes_recvd;
}

int ServerSocket::send_to_socket(uint8_t *buf, size_t length)
{
    if (client_connected)
    {

        int did_send = sendto(sockfd, buf, length,
                              0, (struct sockaddr *)&clientaddr,
                              sizeof(clientaddr));
        return did_send;
    }
    return 0;
}