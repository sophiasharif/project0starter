#include "client_socket.h"

using namespace std;

ClientSocket::ClientSocket(int port, string hostname) : Socket(port)
{
    if (hostname != "localhost")
    {
        servaddr.sin_addr.s_addr = inet_addr(hostname.c_str());
    }
}