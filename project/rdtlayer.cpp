#include "rdtlayer.h"
#include <iostream>
#include "packet.h"
#include <unistd.h>

using namespace std;

RDTLayer::RDTLayer(Socket &sock) : sock(sock)
{
}

void RDTLayer::send_packet()
{
    uint8_t buf[MSS];
    int length = read(0, buf, MSS);
    // if length < 0, don't send anything
    if (length == -1)
        return;

    if (length > MSS)
    {
        cerr << "Packet length exceeds maximum segment size" << endl;
        return;
    }

    Packet p(0, 0, length, false, false, buf);
    uint8_t network_data[PACKET_SIZE];
    int packet_length = p.to_network_data(network_data);
    // cerr << " --- Sending packet ---" << endl;
    // cerr << "Read " << length << " bytes from stdin" << endl;
    // cerr << "Payload: " << buf << endl;
    // cerr << "Packet length: " << packet_length << endl;
    // cerr << "ack: " << network_data[0] << network_data[1] << network_data[2] << network_data[3] << endl;
    // cerr << "seq: " << network_data[4] << network_data[5] << network_data[6] << network_data[7] << endl;
    // cerr << "length: " << network_data[8] << network_data[9] << endl;
    // cerr << "flags: " << network_data[10] << endl;
    // cerr << "unused: " << network_data[11] << endl;
    // cerr << "payload: " << network_data[12] << network_data[13] << network_data[14] << network_data[15] << endl;
    // cerr << "-----------------------" << endl;
    sock.send_to_socket(network_data, packet_length);
}

int RDTLayer::receive_packet()
{
    // attempt to read from socket
    uint8_t network_data[PACKET_SIZE];
    // next step: split into multiple packets if necessary ?
    int bytes_recvd = sock.read_from_socket(network_data, PACKET_SIZE);
    if (bytes_recvd > 0)
    {
        // cerr << "--- Received packet ---" << endl;
        // cerr << "Bytes received: " << bytes_recvd << endl;
        // cerr << "Payload: " << network_data << endl;
        // cerr << "-----------------------" << endl;
        Packet p(network_data, bytes_recvd);
        // p.write_packet_to_stderr();
        packet_buffer.push_back(p);
    }
    return bytes_recvd;
}

int RDTLayer::write_packets()
{
    // write any acknowledged packets into stdout
    if (packet_buffer.size() == 0)
        return 0;
    Packet to_write = packet_buffer[0];
    packet_buffer.erase(packet_buffer.begin());
    write(1, to_write.get_payload(), to_write.get_length());
    return 1;
}
