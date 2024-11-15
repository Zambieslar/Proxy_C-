#include <cstdlib>
#include <stdio.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <unistd.h>
#include "./HttpStatusCodes.h"

#define SOCKET_PORT 8080

int main() {
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
	
	if (bind(sockfd, (struct sockaddr *) &serv_addr, sizeof(serv_addr)) < 0) {
		perror("Failed to bind socket\n");
	} else {
		printf("Socket bound to 0.0.0.0:8080\n");
	}
	listen(sockfd, 5);
	int clilen = sizeof(cli_addr);

	int newsockfd = accept(sockfd, (struct sockaddr *) &cli_addr, (socklen_t *) &clilen);

	if (newsockfd < 0) {
		perror("Accept call failed\n");
	} else {
		int n = read(newsockfd, &buf, 255);
		if (n < 0) {
			perror("Failed to read from socket\n");
		}
		printf("%s", buf);
	}

}
