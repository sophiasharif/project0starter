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

    // construct our address
    my_addr.sin_family = AF_INET;         // use IPv4
    my_addr.sin_addr.s_addr = INADDR_ANY; // accept all connections
    my_addr.sin_port = htons(port);       // let OS choose port
}