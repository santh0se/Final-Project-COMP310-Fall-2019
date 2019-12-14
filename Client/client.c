#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include <sys/socket.h>                       /*  socket-related */
#include <netinet/in.h>                       /*  socket-related */
#include <arpa/inet.h>                        /*  socket-related */

char * cities[] = { "Dummy","Dallas", "Seattle", "Chicago", "Miami", "Phoenix", "Detroit"};  /* dummy is to remove the 0th index */
char client_name[50] = {0};

char * stocks[]   = { "Dummy","Apple", "Microsoft", "Tesla", "Uber", "CVS", "Walgreens"};
int stocks_size = 7;

/*-----------------------------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
    int client_sd;                                              /* sd stands for socket descriptor*/
    int option = 1;
    int bytes_read;
    char read_buffer[READ_BUFFER_SIZE] = {0};                   /* this array is to read the data from client */
    struct sockaddr_in server_address;
    int addr_length = sizeof(server_address);
    char * client_hello = "Hello, this is client!";
    int done = 0;
    
    printf("Starting client.\n");
    
/* ----------------------------------------------------------------------------------------- */
    /* Create Socket Structure ... */
    client_sd = socket(AF_INET, SOCK_STREAM, 0);                 /* create socket structure */
    if(client_sd == 0)                                           /* INET = internet socket */
    {
        printf("Could not create socket (client)!\n");
        exit(0);
    }
    
/* ------------------------------------------------------------------------------------------- */
    
    /* Construct Server Address (IP and PORT) in network bytes mode */
    server_address.sin_family = AF_INET;                                            /* server address (internet) */
    server_address.sin_port = htons(SERVER_PORT);
    
    if(inet_pton(AF_INET, LOCAL_SERVER_IP, &server_address.sin_addr)<=0)            /* use local address only for testing */
    {
        printf("Invalid server address! Could not convert into network bytes!\n");
        exit(0);
    }
    
/* -------------------------------------------------------------------------------------------- */
    
    /* Connecting to server ... */
    printf("Connecting to server: %s at PORT: %d\n", LOCAL_SERVER_IP, SERVER_PORT);
    int result = connect(client_sd, (struct sockaddr *) &server_address, sizeof(server_address));
    if(result < 0)
    {
        printf("Connection to server failed!\n");
        exit(0);
    }
    printf("Connection to server is successful!\n");
    
/* -------------------------------------------------------------------------------------------- */
    
    while(done == 0)
    {
        char line[1024];
        char *buffer = line;
        size_t buffer_size = 1024;
        
        printf("\n Please enter a line/message to send to the server: ");
        if(getline(&buffer, &buffer_size, stdin) == -1)
        {
            done = 1;
            printf("User has terminated! Exiting program! \n");
        }
        else
        {
            /* Start SENDING bytes to server */
            printf("Sending user message to server\n");
            send(client_sd, buffer, strlen(buffer), 0);
            
            /* Start READING bytes from server */
            printf("Reading user message from server!\n");
            bytes_read = read(client_sd, read_buffer, READ_BUFFER_SIZE);
            printf("Bytes sent (echo) by server: %s\n", read_buffer);
        }
    }
    
    printf("Exiting client!\n");
    
}
