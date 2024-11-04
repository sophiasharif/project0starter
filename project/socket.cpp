#include "socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>
#include <fcntl.h>

using namespace std;

Socket::Socket(int port)
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        throw runtime_error("Failed to create socket");
    }
    // make socket non-blocking (update internal fd flags)
    int flags = fcntl(sockfd, F_GETFL, 0);
    fcntl(sockfd, F_SETFL, flags | O_NONBLOCK);
}