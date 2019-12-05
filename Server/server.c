#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>                             /* these are socket-related */
#include <netinet/in.h>
#include "server.h"

int main(int argc, char* argv[])
{
    int server_sd;                                              /* sd stands for socket descriptor*/
    int option = 1;
    int new_connection_sd;
    int bytes_read;
    char read_buffer[READ_BUFFER_SIZE] = {0};                   /* this array is to read the data from client */
    struct sockaddr_in socket_address;
    int addr_length = sizeof(socket_address);
    server_sd = socket(AF_INET, SOCK_STREAM, 0);                /* create socket structure */
    char * server_hello = "Hello, this is server!";
    
    printf("Starting server.\n");
    if(server_sd == 0)                                           /* INET = internet socket */
    {
        printf("Could not create socket!\n");
        exit(0);
    }

/* ---------------------------------------------------------- */
    
    /* Set Socket Option: Re-use ADDRESS and PORT */
    int result = setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
    if(result == 0)
       {
           printf("Could not set socket options!\n");
           exit(0);
       }
    
/* ---------------------------------------------------------- */
    /* BIND the SOCKET to a PORT (2000) */
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;                   /* use any IP of the local server */
    socket_address.sin_port = htons ( PORT );                      /* host to network string */
    
    result = bind(server_sd, (struct sockaddr *) &socket_address, sizeof(socket_address));
    if(result < 0)
      {
        printf("Could not bind to PORT!\n");
        exit(0);
      }

/* -------------------------------------------------------- */
    
    /* Now start LISTENING for connections */
    printf("Set up LISTENING! \n");
    if(listen(server_sd, CONNECTION_BACKLOG) < 0)
    {
        printf("Could not listen!\n");
        exit(0);
    }
    
/* --------------------------------------------------------- */
    /* Start ACCEPTING connections */
    printf("Waiting for new connections ... \n");
    new_connection_sd = accept(server_sd, (struct sockaddr *) &socket_address, (socklen_t*) &addr_length);
    if(new_connection_sd < 0)
    {
        printf("Could not create a new connection (in accept call)!\n");
        exit(0);
    }
    printf("New connection has arrived!\n");
    
/* --------------------------------------------------------- */

    /* Start READING bytes from client */
    
    printf("Waiting for message from client ... \n");
    while((bytes_read = read(new_connection_sd, read_buffer, READ_BUFFER_SIZE)) > 0 )
    {
        read_buffer[bytes_read] = 0;                            /* make it as a string by adding 0 at the end of number of bytes read */
        printf("Bytes sent by client: %s \n", read_buffer);
        
        /*Start SENDING same bytes to client */
        printf("Sending same message back to client!\n");
        send(new_connection_sd, read_buffer, strlen(read_buffer),0);
    }
         
}
  



