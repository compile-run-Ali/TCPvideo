#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFFER_SIZE 1024

int main(){
	char *ip = "127.0.0.1";
	int port = 8080;
	int e;

	// socket file descriptor
	int sock_fd, new_sock;

	// socket address
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[BUFFER_SIZE];

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0) {
		perror("Socket Error");
		exit(1);
	}
	printf("Server socket created.\n");
	 
	 // assign IP, PORT etc
	server_addr.sin_family = AF_INET;

	// convert host byte order to network byte order.
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	

	e = bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(e < 0) {
		perror("Bind Error");
		exit(1);
	}

	e = listen(sock_fd, 5);
	if(e < 0) {
		perror("Listen Error");
		exit(1);
	}

	printf("Listening on port %d...\n", port);

	addr_size = sizeof(new_addr);
	new_sock = accept(sock_fd, (struct sockaddr*)&new_addr, &addr_size);
	if(new_sock < 0) {
		perror("Accept Error");
		exit(1);
	}

	printf("Connection accepted from %s:%d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));

	// // receive file
	// FILE *fp;
	// char *filename = "test.txt";
	// fp = fopen(filename, "w");
	// if(fp == NULL) {
	// 	perror("File Error");
	// 	exit(1);
	// }

	bzero(buffer, BUFFER_SIZE);
	int words;


	return 0;
}