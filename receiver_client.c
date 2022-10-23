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

    // recive file and save it
    FILE *file_pointer;
    int recieved_bytes = 0;
    char recv_buff[1024];
    memset(recv_buff, '0', sizeof(recv_buff));
    // open new file
    file_pointer = fopen("received_file.mp4", "ab");
    if(NULL == file_pointer)
    {
        printf("Error opening file");
        return 1;
    }

    // recieve in 128 bits chunks
    int i = 0 ;

    // when read() <= 0 then EOF has been reached
    while((recieved_bytes = read(sock_fd, recv_buff, 1024)) > 0)
    {   i++;
        fwrite(recv_buff, 1,recieved_bytes,file_pointer);
    }

    // number of segments recieved
    printf("Number of buffer cycles = %d\n", i);

    if(recieved_bytes < 0)
    {
        printf("\n Could not read recived bytes\n");
    }

    printf("File recived and saves as recieved_file.mp4\n");
    return 0;

}



