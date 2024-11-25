#include "./HttpStatusCodes.h"
#include "./StateMachine.h"
#include <cstdlib>
#include <netinet/in.h>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>

#define SOCKET_PORT 8080

int main() {
  Parser::StateMachine StateMachine;
  char buf[1024];
  sockaddr_in serv_addr, cli_addr;
  int sockfd = socket(AF_INET, SOCK_STREAM, 0);
  serv_addr.sin_family = AF_INET;
  serv_addr.sin_port = htons(SOCKET_PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  if (sockfd < 0) {
    perror("Failed to create socket\n");
  } else {
    printf("Created socket\n");
  }

  while (bind(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) {
    sleep(2);
    perror("Failed to bind socket");
  }
  printf("Socket bound to 0.0.0.0:8080\n");

  listen(sockfd, 5);
  int clilen = sizeof(cli_addr);

  int newsockfd =
      accept(sockfd, (struct sockaddr *)&cli_addr, (socklen_t *)&clilen);

  if (newsockfd < 0) {
    perror("Accept call failed\n");
  } else {
    int n = read(newsockfd, &buf, 1023);

    if (n < 0) {
      perror("Failed to read from socket\n");
    }
    StateMachine.run(buf, n);
    close(sockfd);
    close(newsockfd);
  }
  return 0;
}
