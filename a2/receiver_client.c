// include the header files
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>
#include <unistd.h>


#define BUFFER_SIZE 1024

int main(){
    // assign IP, PORT numbers to variables
    char ip[] = "127.0.0.1";
    int port = 8080;
    int e;

    // make client socket
    int sock_fd;
    struct sockaddr_in server_addr;
    char buffer[BUFFER_SIZE];


    sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    if(sock_fd < 0) {
        perror("Error in socket");
        exit(1);
    }
    printf("Client socket created.\n");

    // assign IP, PORT etc
    server_addr.sin_family = AF_INET;
    server_addr.sin_port = htons(port);
    server_addr.sin_addr.s_addr = inet_addr(ip);

    e = connect(sock_fd, (struct sockaddr*)&server_addr, sizeof(server_addr));
    if(e < 0) {
        perror("Error while connecting");
        exit(1);
    }

    printf("Established connection with %s:%d\n", ip, port);

    // get command from user
    printf("Enter command: ");
    fgets(buffer, BUFFER_SIZE, stdin);
    buffer[strlen(buffer) - 1] = '\0';

    // send command to server
    e = send(sock_fd, buffer, strlen(buffer), 0);
    if(e < 0) {
        perror("Error while sending");
        exit(1);
    }

    // receive response from server
    e = recv(sock_fd, buffer, BUFFER_SIZE, 0);
    if(e < 0) {
        perror("Error while receiving");
        exit(1);
    }
    buffer[e] = '\0';

    printf("Response from server: %s\n", buffer);


    // close the socket
    close(sock_fd);
    printf("Socket closed\n");




    return 0;

}



