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
   RDTLayer rdt(socket);
   int BUF_SIZE = 1024;

   while (1)
   {
      uint8_t their_buf[BUF_SIZE];
      int bytes_recvd = socket.read_from_socket(their_buf, BUF_SIZE);
      if (bytes_recvd > 0)
         write(1, their_buf, bytes_recvd);

      uint8_t my_buf[BUF_SIZE];
      int bytes_read = read(0, my_buf, BUF_SIZE);
      rdt.send_packet(my_buf, bytes_read, 0, 0, false, false);
   }

   return 0;
}