#include "rdtlayer.h"
#include <iostream>

using namespace std;

RDTLayer::RDTLayer(Socket &sock) : sock(sock)
{
}

void RDTLayer::send_packet(uint8_t *buf, int length, uint32_t ack, uint32_t seq, bool ack_bit, bool syn_bit)
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

int RDTLayer::receive_packet(uint8_t *buf, int buf_size)
{
    int bytes_recvd = sock.read_from_socket(buf, buf_size);
    return bytes_recvd;
}