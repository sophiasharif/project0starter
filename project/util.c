#include "util.h"
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>

socklen_t serversize = sizeof(socklen_t); // Temp buffer for recvfrom API

int BUF_SIZE = 1024;

int send_packet(int sockfd, char server_buf[BUF_SIZE], struct sockaddr_in serveraddr)
{
    /* 5. Listen for response from server */
    int bytes_recvd = recvfrom(sockfd, server_buf, BUF_SIZE,
                               // socket  store data  how much
                               0, (struct sockaddr *)&serveraddr,
                               &serversize);
    return bytes_recvd;
}