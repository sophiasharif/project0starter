#include "util.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

int BUF_SIZE = 1024;

int read_from_socket(int sockfd, char server_buf[BUF_SIZE], struct sockaddr_in serveraddr, socklen_t address_size)
{
    /* 5. Listen for response from server */
    int bytes_recvd = recvfrom(sockfd, server_buf, BUF_SIZE,
                               // socket  store data  how much
                               0, (struct sockaddr *)&serveraddr,
                               &address_size);
    return bytes_recvd;
}