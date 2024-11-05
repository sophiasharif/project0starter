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
    int bytes_recvd = recvfrom(sockfd, server_buf, BUF_SIZE,
                               // socket  store data  how much
                               0, (struct sockaddr *)&servaddr,
                               &address_size);
    if (bytes_recvd > 0)
        write(1, server_buf, bytes_recvd);
}

void ClientSocket::send_to_socket()
{
    char client_buf[1024];

    int bytes_read = read(0, client_buf, 1024);
    if (bytes_read > 0)
    {
        int did_send = sendto(sockfd, client_buf, bytes_read,
                              // socket  send data   how much to send
                              0, (struct sockaddr *)&(servaddr),
                              // flags   where to send
                              sizeof(servaddr));
        if (did_send < 0)
        {
            throw runtime_error("Error sending data to server");
        }
    }
}