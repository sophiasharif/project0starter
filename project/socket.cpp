#include "socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <fcntl.h>
#include <unistd.h>

using namespace std;

Socket::Socket(int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        throw runtime_error("Failed to create socket");
    }

    // make non-blocking (update internal fd flags)
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK); // socket
    fcntl(0, F_SETFL, flags | O_NONBLOCK);      // stdin

    // set up server address
    servaddr.sin_family = AF_INET;         // use IPv4
    servaddr.sin_addr.s_addr = INADDR_ANY; // accept all connections
    servaddr.sin_port = htons(port);       // Big endian
}

Socket::~Socket()
{
    close(sockfd);
}