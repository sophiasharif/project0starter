#include "socket.h"
#include <sys/socket.h>
#include <arpa/inet.h>
#include <stdexcept>

using namespace std;

Socket::Socket()
{
    sockfd = socket(AF_INET, SOCK_DGRAM, 0);
    if (sockfd < 0)
    {
        throw runtime_error("Failed to create socket");
    }
}