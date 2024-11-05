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

void ServerSocket::read_from_socket()
{
    char client_buf[BUF_SIZE];
    socklen_t address_size = sizeof(clientaddr);
    int bytes_recvd = recvfrom(sockfd, client_buf, BUF_SIZE,
                               0, (struct sockaddr *)&clientaddr,
                               &address_size);
    if (bytes_recvd > 0)
    {
        client_connected = 1;
        write(1, client_buf, bytes_recvd);
    }
}

void ServerSocket::send_to_socket()
{
    if (client_connected)
    {
        char server_buf[BUF_SIZE];
        int bytes_read = read(0, server_buf, BUF_SIZE);
        int did_send = sendto(sockfd, server_buf, bytes_read,
                              0, (struct sockaddr *)&clientaddr,
                              sizeof(clientaddr));
        // if (did_send < 0)
        // {
        //     std::cerr << "Error sending data to client" << std::endl;
        //     std::cerr << errno << std::endl;
        //     throw runtime_error("Failed to send data to client");
        // }
    }
}