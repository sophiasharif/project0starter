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
   int port = atoi(argv[1]);
   ServerSocket socket(port);

   // make stdin non-blocking
   int flags = fcntl(0, F_GETFL, 0);
   fcntl(0, F_SETFL, flags | O_NONBLOCK);

   while (1)
   {
      socket.read_from_socket();
      // read from stdin
      if (socket.client_connected)
      {
         char server_buf[1024];
         int bytes_read = read(0, server_buf, 1024);
         int did_send = sendto(socket.get_sockfd(), server_buf, bytes_read,
                               // socket  send data   how much to send
                               0, (struct sockaddr *)&socket.clientaddr,
                               // flags   where to send
                               sizeof(socket.clientaddr));
         // if (did_send < 0)
         //    return errno;
      }
   }

   /* 7. Send data back to client */
   return 0;
}