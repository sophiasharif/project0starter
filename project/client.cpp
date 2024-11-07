#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <openssl/evp.h>
#include <fcntl.h>
#include "util.h"
#include "client_socket.h"
#include "rdtlayer.h"
#include <iostream>

using namespace std;

int main(int argc, char **argv)
{
   if (argc != 3)
   {
      printf("Usage: %s <hostname> <port>\n", argv[0]);
      exit(1);
   }

   int BUF_SIZE = 1024;
   ClientSocket socket(atoi(argv[2]), argv[1]);
   RDTLayer rdt(socket);

   while (1)
   {
      uint8_t their_buf[BUF_SIZE];
      int bytes_recvd = rdt.receive_packet(their_buf, BUF_SIZE);
      if (bytes_recvd > 0)
         write(1, their_buf, bytes_recvd);

      uint8_t my_buf[BUF_SIZE];
      int bytes_read = read(0, my_buf, BUF_SIZE);
      rdt.send_packet(my_buf, bytes_read, 0, 0, false, false);
   }
   return 0;
}

/*
switch(client_state) {
   case CLIENT_START:
      send packet with:
         - sequence number = random number between 0 and half of the max size
         - ack number = 0
         - length = 0
         - flags = 0b10000000 (SYN)
         - payload = NULL
   case CLIENT_AWAIT:
      send packet with:
         - sequence number = SEQ + 1 (client sequence number + 1)
         - ack number = server sequence number + 1
         - flags = 0b01000000 (ACK)
         - length = 0
         - payload = NULL
   case CLIENT_AWAIT_2:
   case CONNECTED:
}

switch(server_state) {
   case SERVER_START:
      send packet with:
         - sequence number = random number between 0 and half of the max size
         - ack number = client sequence number + 1
         - length = 0
         - flags = 0b11000000 (SYN+ACK)
         - payload = NULL
   case SERVER_SYN:
   case SERVER_AWAIT_2:
   case CONNECTED:
}
*/