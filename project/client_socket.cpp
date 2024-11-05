#include "client_socket.h"
#include <unistd.h>

using namespace std;

ClientSocket::ClientSocket(int port, string hostname) : Socket(port)
{
    if (hostname != "localhost")
    {
        servaddr.sin_addr.s_addr = inet_addr(hostname.c_str());
    }
}

void ClientSocket::read_from_socket()
{
    char server_buf[1024];
    socklen_t address_size = sizeof(socklen_t);
    int bytes_recvd = recvfrom(sockfd, server_buf, 1024,
                               // socket  store data  how much
                               0, (struct sockaddr *)&servaddr,
                               &address_size);
    if (bytes_recvd > 0)
        write(1, server_buf, bytes_recvd);
}