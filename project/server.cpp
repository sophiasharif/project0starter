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

   while (1)
   {
      socket.read_from_socket();
      socket.send_to_socket();
   }

   return 0;
}