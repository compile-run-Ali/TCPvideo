#include <arpa/inet.h>
#include <stdio.h>
#include <string.h>
#include <sys/socket.h>
#include <unistd.h>
#include <stdlib.h>
#define SIZE 1024
void sendfile(FILE *fp, int sockfd){
  int n;
  char data[SIZE] = {0};
 
  while(fgets(data, SIZE, fp) != NULL) {
    if (send(sockfd, data, sizeof(data), 0) == -1) {
      perror("[-]Error in sending file.");
      exit(1);
    }
    bzero(data, SIZE);
  }
}

int main() {
	const char* server_name = "localhost";
	const int server_port = 8877;

	struct sockaddr_in server_address;
	memset(&server_address, 0, sizeof(server_address));
	server_address.sin_family = AF_INET;

	// creates binary representation of server name
	// and stores it as sin_addr
	// http://beej.us/guide/bgnet/output/html/multipage/inet_ntopman.html
	inet_pton(AF_INET, server_name, &server_address.sin_addr);

	// htons: port in network order format
	server_address.sin_port = htons(server_port);

	// open a stream socket
	int sock;
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		printf("could not create socket\n");
		return 1;
	}

	// TCP is connection oriented, a reliable connection
	// **must** be established before any data is exchanged
	if (connect(sock, (struct sockaddr*)&server_address,
	            sizeof(server_address)) < 0) {
		printf("could not connect to server\n");
		return 1;
	}

	// we need to send video file


	// video that will be sent to the server
	FILE *f;
	f = fopen("sample.mp4", "r");
	if (f == NULL) {
		printf("Error opening file\n");
		exit(1);
	}
	sendfile(f, sock);
	printf("File data sent successfully.\n");
	fclose(f);

	// receive

	

	// // will remain open until the server terminates the connection
	// while ((n = recv(sock, pbuffer, maxlen, 0)) > 0) {
	// 	pbuffer += n;
	// 	maxlen -= n;
	// 	len += n;

	// 	buffer[len] = '\0';
	// 	printf("received: '%s'\n", buffer);
	// }

	// close the socket
	close(sock);
	return 0;
}
