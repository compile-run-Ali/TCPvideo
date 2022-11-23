// include headerfiles which are needed
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFFER_SIZE 1024

int main(){
	char ip[] = "127.0.0.1";
	int port = 8080;
	int e;

	// socket file descriptor
	int sock_fd, new_sock;

	// socket address
	struct sockaddr_in server_addr, new_addr;
	socklen_t addr_size;
	char buffer[BUFFER_SIZE];
	char result[BUFFER_SIZE];

	sock_fd = socket(AF_INET, SOCK_STREAM, 0);
	if(sock_fd < 0) {
		perror("Socket Error");
		exit(1);
	}
	printf("Socket for server created.\n");
	 
	 // assign IP, PORT etc
	server_addr.sin_family = AF_INET;

	// convert host byte order to network byte order.
	server_addr.sin_port = htons(port);
	server_addr.sin_addr.s_addr = htonl(INADDR_ANY);
	
	// bind the socket to the set address
	e = bind(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
	if(e < 0) {
		perror("Error while binding");
		exit(1);
	}

	// listen for connection and queu up to 5 requests

	e = listen(sock_fd, 5);
	if(e < 0) {
		perror("Error while listening");
		exit(1);
	}


	printf("Listening on port %d...\n", port);


	// when connection is established, accept the connection
	addr_size = sizeof(new_addr);
	new_sock = accept(sock_fd, (struct sockaddr*)&new_addr, &addr_size);
	if(new_sock < 0) {
		perror("Accept Error");
		exit(1);
	}

	printf("Connection accepted from %s:%d\n", inet_ntoa(new_addr.sin_addr), ntohs(new_addr.sin_port));

	
	

   // read the command from client and execute it using system() and then send the output back to client
   e = recv(new_sock, buffer, BUFFER_SIZE, 0);
   if(e < 0) {
	  perror("Error while receiving");
	  exit(1);
   }
   buffer[e] = '\0';
   printf("Command received: %s, executing it...\n", buffer);
   //execute and store in result
   FILE *fp = popen(buffer, "r");
   if(fp == NULL) {
	   perror("Error while executing");
	   exit(1);
   }

   // send the fp file back to client
   while(fgets(result, BUFFER_SIZE, fp) != NULL) {
	   e = send(new_sock, result, strlen(result), 0);
	   if(e < 0) {
		   perror("Error while sending");
		   exit(1);
	   }
   }
   pclose(fp);
   

   printf("Result sent to client.\n");

   // close the socket

	// close connection and socket when file end reached
	printf("File closed.\n");


	close(new_sock);
	close(sock_fd);
	printf("Socket closed.\n");
	printf("File sent successfully.\n");

	


	return 0;

}

