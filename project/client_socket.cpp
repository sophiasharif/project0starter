#include "client_socket.h"
#include <unistd.h>
#include <stdexcept>

using namespace std;

ClientSocket::ClientSocket(int port, string hostname) : Socket(port)
{
    if (hostname != "localhost")
    {
        servaddr.sin_addr.s_addr = inet_addr(hostname.c_str());
    }
}

int ClientSocket::read_from_socket(uint8_t *buf, size_t length)
{

    socklen_t address_size = sizeof(socklen_t);
    int bytes_recvd = recvfrom(sockfd, buf, length,
                               // socket  store data  how much
                               0, (struct sockaddr *)&servaddr,
                               &address_size);
    return bytes_recvd;
}

int ClientSocket::send_to_socket(uint8_t *buf, size_t length)
{
    int did_send = sendto(sockfd, buf, length,
                          // socket  send data   how much to send
                          0, (struct sockaddr *)&(servaddr),
                          // flags   where to send
                          sizeof(servaddr));
    if (did_send < 0)
    {
        throw runtime_error("Error sending data to server");
    }
    return did_send;
}