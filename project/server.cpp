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

int main(int argc, char **argv)
{
   if (argc != 2)
   {
      fprintf(stderr, "Usage: %s <port>\n", argv[0]);
      return 1;
   }

   ServerSocket socket(atoi(argv[1]));
   const int BUF_SIZE = 1024;

   while (1)
   {
      char client_buf[BUF_SIZE];
      int bytes_recvd = socket.read_from_socket(client_buf, BUF_SIZE);
      if (bytes_recvd > 0)
         write(1, client_buf, bytes_recvd);

      char server_buf[BUF_SIZE];
      int bytes_read = read(0, server_buf, BUF_SIZE);
      socket.send_to_socket(server_buf, bytes_read);
   }

   return 0;
}