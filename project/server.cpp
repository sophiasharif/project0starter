#include <sys/socket.h>
#include <arpa/inet.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>
#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include "util.h"
#include "server_socket.h"
#include "rdtlayer.h"

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s <port>\n", argv[0]);
      return 1;
   }

   ServerSocket socket(atoi(argv[1]));
   RDTLayer rdt(socket, SERVER);

   while (1)
   {
      rdt.receive_packet();
      rdt.write_packets();
      rdt.read_packet();
      rdt.send_packet();
   }

   return 0;
}