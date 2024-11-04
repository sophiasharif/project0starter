#include <sys/socket.h>
#include <arpa/inet.h>

int read_from_socket(int sockfd, char server_buf[1024], struct sockaddr_in serveraddr, socklen_t address_size);