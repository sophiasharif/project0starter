#include "server_socket.h"
#include <stdexcept>

using namespace std;

ServerSocket::ServerSocket(int port) : Socket(port)
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