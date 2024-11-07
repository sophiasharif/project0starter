#include "rdtlayer.h"
#include <iostream>
#include "packet.h"
#include <unistd.h>

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

    uint8_t network_data[PACKET_SIZE];
    Packet p(ack, seq, length, ack_bit, syn_bit, buf);
    int packet_length = p.to_network_data(network_data);
    cerr << "Packet length: " << packet_length << endl;
    cerr << "ack: " << network_data[0] << network_data[1] << network_data[2] << network_data[3] << endl;
    cerr << "seq: " << network_data[4] << network_data[5] << network_data[6] << network_data[7] << endl;
    cerr << "length: " << network_data[8] << network_data[9] << endl;
    cerr << "flags: " << network_data[10] << endl;
    cerr << "unused: " << network_data[11] << endl;
    cerr << "payload: " << network_data[12] << network_data[13] << network_data[14] << network_data[15] << endl;
    sock.send_to_socket(network_data, packet_length);
}

int RDTLayer::receive_packet(uint8_t *buf, int buf_size)
{
    uint8_t network_data[PACKET_SIZE];
    int bytes_recvd = sock.read_from_socket(network_data, PACKET_SIZE);
    if (bytes_recvd > 0)
    {
        cerr << "Bytes received: " << bytes_recvd << endl;
        cerr << "Payload: " << network_data << endl;
        Packet p(network_data, bytes_recvd);
        cerr << "ack: " << p.get_ack() << endl;
        cerr << "seq: " << p.get_seq() << endl;
        cerr << "length: " << p.get_length() << endl;
        cerr << "flags: " << p.is_ack_set() << p.is_syn_set() << endl;
        cerr << "payload: " << p.get_payload() << endl;
        write(1, p.get_payload(), p.get_length());
        memcpy(buf, p.get_payload(), p.get_length());
    }
    return bytes_recvd;
}