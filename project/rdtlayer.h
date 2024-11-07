#ifndef RDTLAYER_H
#define RDTLAYER_H

#include "socket.h"
#include "packet.h"

class RDTLayer
{
public:
    RDTLayer(Socket &sock);
    void send_packet(uint8_t *buf, int length, uint32_t ack, uint32_t seq, bool ack_bit, bool syn_bit);
    int receive_packet(uint8_t *buf, int buf_size);

private:
    Socket &sock;
};

#endif