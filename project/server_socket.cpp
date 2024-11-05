#include "server_socket.h"
#include <stdexcept>
#include <unistd.h>

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