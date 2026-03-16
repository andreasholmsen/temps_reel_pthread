/*
 * Description:  A simple client program to connect to the TCP/IP server thanks to Darren Smith
 */

/*
 * Linux:   compile with gcc -Wall client.c -o client
 */

#include <stdio.h>
#include <string.h>

#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#include <time.h>
#include <assert.h>
#include <errno.h>

#define SOCKET_PORT 10020
#define SOCKET_SERVER "127.0.0.1" /* local host */

int fd; 


int main(int argc, char *argv[]) {
  struct sockaddr_in address;
  const struct hostent *server;
  int rc;
  char buffer[256];

  /* create the socket */
  fd = socket(AF_INET, SOCK_STREAM, 0);
  if (fd == -1) {
    printf("cannot create socket\n");
    return -1;
  }

  /* fill in the socket address */
  memset(&address, 0, sizeof(struct sockaddr_in));
  address.sin_family = AF_INET;
  address.sin_port = htons(SOCKET_PORT);
  server = gethostbyname(SOCKET_SERVER);

  if (server)
    memcpy((char *)&address.sin_addr.s_addr, (char *)server->h_addr, server->h_length);
  else {
    printf("cannot resolve server name: %s\n", SOCKET_SERVER);
    close(fd);
    return -1;
  }

  /* connect to the server */
  rc = connect(fd, (struct sockaddr *)&address, sizeof(struct sockaddr));
  if (rc == -1) {
    printf("cannot connect to the server\n");
    close(fd);
    return -1;
  }

  fflush(stdout);

  send(fd,"M,99,99\n",strlen("M,99,99\n"),0);

  for (;;) {
    int n = recv(fd, buffer, 256, 0);
    buffer[n] = '\0';
    printf("Received: %s", buffer);
  }

  close(fd);

  return 0;
}
