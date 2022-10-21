#include <arpa/inet.h>
#include <netinet/in.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <time.h>
#define SIZE 1024
/**
 * TCP Uses 2 types of sockets, the connection socket and the listen socket.
 * The Goal is to separate the connection phase from the data exchange phase.
 * */
void writefile(int sock){
	int n;
	FILE *fp;
	char *filename = "received.mp4";
	char buffer[SIZE];
	fp = fopen(filename, "w");
	while(1){
		n = recv(sock, buffer, SIZE, 0);
		if (n <= 0){
			break;
			return;
		}
		fprintf(fp, "%s", buffer);
		bzero(buffer, SIZE);
	}
	return;

}

int main(int argc, char *argv[]) {
	// port to start the server on
	int SERVER_PORT = 8877;
           socklen_t client_address_len;

	socklen_t addr_size;
	// socket address used for the server
	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// htons: host to network short: transforms a value in host byte
	// ordering format to a short value in network byte ordering format
	server_address.sin_port = htons(SERVER_PORT);

	// htonl: host to network long: same as htons but to long
	server_address.sin_addr.s_addr = htonl(INADDR_ANY);

	// create a TCP socket, creation returns -1 on failure
	int listen_sock;
	if ((listen_sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("could not create listen socket\n");
		return 1;
	}
	printf("listen socket created\n");

	// bind it to listen to the incoming connections on the created server
	// address, will return -1 on error
	if ((bind(listen_sock, (struct sockaddr *)&server_address,
	          sizeof(server_address))) < 0) {
		printf("could not bind socket\n");
		return 1;
	}
	printf("bind done\n");

	int wait_size = 16;  // maximum number of waiting clients, after which
	                     // dropping begins
	if (listen(listen_sock, wait_size) < 0) {
		printf("could not open socket for listening\n");
		return 1;
	}
	printf("server is listening\n");
	//we have to recieve video files

	// socket address used to store client address
	struct sockaddr_in client_address;
	
	addr_size=sizeof(client_address);
	int new_socket;
	new_socket = accept(listen_sock, (struct sockaddr *)&client_address, &addr_size);
	writefile(new_socket);
	printf("file received\n");

	// run indefinitely
	// while (true) {
	// 	// open a new socket to transmit data per connection as audio files
	// 	socklen_t addr_size=sizeof(client_address);
	// 	int client_sock = accept(listen_sock, (struct sockaddr *)&client_address, &addr_size);
	// 	printf("client connected\n");

	// }

	close(listen_sock);
	return 0;
}
