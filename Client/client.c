#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <string.h>
#include "client.h"
#include <sys/socket.h>                             /* these are socket-related */
#include <netinet/in.h>
#include <arpa/inet.h>

char * cities[] = { "Dummy","Dallas", "Seattle", "Chicago", "Miami", "Phoenix", "Detroit"};  /* dummy is to remove the 0th index */
char client_name[50] = {0};

char * stocks[]   = { "Dummy","Apple", "Microsoft", "Tesla", "Uber", "CVS", "Walgreens"};
int stocks_size = 7;

/*--------------------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
    int client_sd;                                              /* sd stands for socket descriptor*/
    int option = 1;
    int bytes_read;
    char read_buffer[READ_BUFFER_SIZE] = {0};                   /* this array is to read the data from client */
    struct sockaddr_in server_address;
    int addr_length = sizeof(server_address);
    char * client_hello = "Hello, this is client!\n";
    int done = 0;
    int server_port = SERVER_PORT;
    int client_type = -1;
    
    /*printf("DEBUG: %d",argc);*/
    
    if(argc < 3)  /* num args less than 3 = error */
      {
        printf("Usage: ./runclient <client-name> W/S [port-number]\n");
        exit(1);
      }
 
    if(argc == 4) /* if optional port provided*/
      {
        printf("DEBUG ARGV[3]: %s\n", argv[3]);
        server_port = atoi(argv[3]);
      }
    else
      {
        server_port = SERVER_PORT;
      }
    
    strcpy(client_name, argv[1]);
    printf("CLIENT NAME: %s\n", client_name);
   
    if(strcmp(argv[2],"W") == 0)
      {
        printf("Starting WEATHER CLIENT...\n");
        client_type = 1;
      }
    else if (strcmp(argv[2],"S")==0)
      {
        printf("Starting STOCK CLIENT...\n");
        client_type = 2;
      }
    else
      {
        printf("Unknown client type argument %s\n",argv[2]);
        exit(1);
      }
 
 /*-----------------------------------------------*/
    
    /* Create Socket Structure ... */
    
    client_sd = socket(AF_INET, SOCK_STREAM, 0);                 /* create socket structure */
    if(client_sd == 0)                                           /* INET = internet socket */
      {
        printf("Could not create socket (client)!\n");
        exit(0);
      }
    
 /*------------------------------------------------*/
    
    /* Construct Server Address (IP and PORT) in network bytes mode ... */
    
    server_address.sin_family = AF_INET;                                            /* server address (internet) */
    server_address.sin_port = htons(server_port);
    
    if(inet_pton(AF_INET, LOCAL_SERVER_IP, &server_address.sin_addr)<=0)            /* use local address only for testing */
      {
        printf("Invalid server address! Could not convert into network bytes!\n");
        exit(0);
      }
    
  /*------------------------------------------------*/
    
    /* Connecting to server ... */
    
    printf("Connecting to server: %s at PORT: %d\n", LOCAL_SERVER_IP, server_port);
    
    int result = connect(client_sd, (struct sockaddr *) &server_address, sizeof(server_address));
    if(result < 0)
      {
        printf("Connection to server failed!\n");
        exit(0);
      }
    printf("Connection to server is successful!\n");
    
  /*-------------------------------------------------*/
    
    while(done == 0)
    {
        char line[1024] = {0};
        char message[1024] = {0};
        char *buffer = line;
        size_t buffer_size = 1024;
        
        if(client_type == 1)
          {
             display_weather_menu();
          }
        else if(client_type == 2)
          {
            display_stock_menu();
          }
        else
          {
            printf("Unknown client type: %d\n", client_type);
            exit(1);
          }
        
        int res = getline(&buffer, &buffer_size, stdin);
        buffer[1] = 0;                                      /* the next element must be 0 in order for strcmp to work*/
        int option = atoi(buffer);
        /*printf("User Option: %d\n", option);*/
        
        if(option < 1 || option > 9)
          {
            printf("Unknown option. Please enter a number between 1-7.\n");
            continue;
          }
        if(res == -1 || strcmp(buffer, "7") == 0)
          {
            done = 1;
            printf("User has terminated! Exiting program! \n");
            exit(1);
          }
        else
          {
            /* Start SENDING bytes to server */
            if(client_type == 1)
              {
                printf("Client: Sending user message to server: %s ... \n", cities[option]);
                sprintf(message, "%s %s",client_name, cities[option]);
              }
            else if(client_type == 2)
              {
                printf("Client: Sending user message to server: %s ... \n", stocks[option]);
                sprintf(message, "%s %s",client_name, stocks[option]);
              }
              
            send(client_sd, message, strlen(message),0);
                 
            /* Start READING bytes from server */
            printf("Server: Reading user message ...\n");
            bytes_read = read(client_sd, read_buffer, READ_BUFFER_SIZE);
            printf("*****************************************************\n");
            printf("Server Response: %s", read_buffer);
            printf("*****************************************************\n");
            printf("Press ENTER/RETURN to continue!");
            getchar();  /* it means get character, waits for user to press anything */
         }
     }
 printf("Exiting client!\n");
}

/*-------------------------------------------------------------------------------------------------------------------------------------------*/

/* This function will display the options to select to send to server & server will return back result */

void display_weather_menu()
{
    char *menu = "\n\n\n\
                Weather Forecast:\n\
                    1. Dallas\n\
                    2. Seattle\n\
                    3. Chicago\n\
                    4. Miami\n\
                    5. Phoenix\n\
                    6. Detroit\n\
                    7. QUIT PROGRAM\n\
                 \n";
    
     printf("%s", menu);
     printf("Please enter a number between 1-6 to display the weather of the selected city (or press 7 to quit): ");
}

/*---------------------------------------------------------------------------------------------------------------------------------------------*/

/* This function will display the options to select to send to server & server will return back result */

void display_stock_menu()
{
    char *menu = "\n\n\n\
                Stock Ticker:\n\
                    1. Apple\n\
                    2. Microsoft\n\
                    3. Tesla\n\
                    4. Uber\n\
                    5. CVS\n\
                    6. Walgreens\n\
                    7. QUIT PROGRAM\n\
                 \n";
    
     printf("%s", menu);
     printf("Please enter a number between 1-6 to display the stock/share value of the selected company (or press 7 to quit): ");
}
