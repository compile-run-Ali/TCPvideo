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

	
	

	FILE *file_pointer;
	char *filename = "sample.mp4";
	file_pointer = fopen(filename, "rb");
	if(file_pointer == NULL) {
		perror("Error: Could not open fil,e");
		exit(1);
	}

	printf("Reading data from file and sending it...\n");
	while(1) {
		// read data from file and send it
		// transfer data to buffer
		int bytes_read = fread(buffer, 1, BUFFER_SIZE, file_pointer);

		
		// send data
		if(send(new_sock, buffer, bytes_read, 0) < 0) {
			perror("Error while sending file");
			exit(1);
		}

		// check if end of file
		if(bytes_read < BUFFER_SIZE) {
			if(feof(file_pointer)) {
				printf("File end reached.\n");
			}
			if(ferror(file_pointer)) {
				printf("Read error.\n");
			}

			break;

		}

	}

	printf("File sent successfully.\n");

	fclose(file_pointer);

	return 0;

}

