#include "rdtlayer.h"
#include <iostream>

using namespace std;

RDTLayer::RDTLayer(Socket &sock) : sock(sock)
{
}

void RDTLayer::send_packet(char *buf, int length)
{
    // if length < 0, don't send anything
    if (length == -1)
        return;

    if (length > MSS)
    {
        cerr << "Packet length exceeds maximum segment size" << endl;
        return;
    }
    sock.send_to_socket(buf, length);
}

void RDTLayer::receive_packet(char *buf, int length)
{
    sock.read_from_socket(buf, length);
}