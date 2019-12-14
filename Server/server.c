#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <sys/socket.h>                                     /* these are socket-related */
#include <netinet/in.h>
#include <time.h>
#include "server.h"

char * cities[]   = { "Dummy","Dallas", "Seattle", "Chicago", "Miami", "Phoenix", "Detroit"};
int cities_size = 7;
int temperatures[] = { -1, 72, 55, 40, 75, 50, 76};

char * stocks[]   = { "Dummy","Apple", "Microsoft", "Tesla", "Uber", "CVS", "Walgreens"};
int stocks_size = 7;
float stock_prices[] = {-1.0, 59.21, 55.23, 40.23, 75.21, 60.00, 60.72};

int client_count = 0;
int new_connection_sd;
int port = PORT;

/*------------------------------------------------------------------------------------------------------------------------------------------------*/

int main(int argc, char* argv[])
{
    int server_sd;                                              /* sd stands for socket descriptor*/
    int server_type=-1;
    int option = 1;
    struct sockaddr_in socket_address;
    int addr_length = sizeof(socket_address);
    server_sd = socket(AF_INET, SOCK_STREAM, 0);                /* create socket structure */
    char * server_hello = "Hello, this is server!";
    pthread_attr_init(&attr);
    pthread_attr_setdetachstate(&attr, PTHREAD_CREATE_JOINABLE);
    
    if(argc < 2)
      {
        printf(" Usage: ./runserver W/S [<port number>]\n");
        exit(1);
      }
    if(argc == 3)
      {
        port = atoi(argv[2]);
      }
    if( strcmp(argv[1],"W")==0)
      {
        printf("Starting WEATHER server...\n");
        server_type = 1;
      }
    else if (strcmp(argv[1],"S")==0)
      {
        printf("Starting STOCK server...\n");
        server_type = 2;
      }
    else
      {
        printf("Unkown server type arguement %s\n",argv[1]);
        exit(1);
      }
        
    if(server_sd == 0)                                           /* INET = internet socket */
      {
        printf("Could not create socket!\n");
        exit(1);
      }

 /*-----------------------------------------*/
    
    /* Set Socket Option: Re-use ADDRESS and PORT */
    
    int result = setsockopt(server_sd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT, &option, sizeof(option));
    if(result == 0)
      {
        printf("Could not set socket options!\n");
        exit(0);
      }
    
 /*------------------------------------------*/
    
    /* BIND the SOCKET to a PORT (2000) */
    
    socket_address.sin_family = AF_INET;
    socket_address.sin_addr.s_addr = INADDR_ANY;                   /* use any IP of the local server */
    socket_address.sin_port = htons ( port );                      /* host to network string */
    
    printf("Binding to port: %d\n", port);
    result = bind(server_sd, (struct sockaddr *) &socket_address, sizeof(socket_address));
    if(result < 0)
      {
        printf("Could not bind to PORT %d!\n", port);
        exit(0);
      }

/*------------------------------------------*/
    
    /* Now start LISTENING for connections  ... */
    
    printf("Set up LISTENING! \n");
    if(listen(server_sd, CONNECTION_BACKLOG) < 0)
      {
        printf("Could not listen!\n");
        exit(0);
      }
    
/*------------------------------------------*/
    
    /* Start ACCEPTING connections  */
    
    while(1)
    {
        printf("Waiting for new connections ... \n");
        new_connection_sd = accept(server_sd, (struct sockaddr *) &socket_address, (socklen_t*) &addr_length);
        printf("NEW CONNECTION SD: %d\n", new_connection_sd);
        
        if(new_connection_sd < 0)
          {
            printf("Could not create a new connection (in accept call)!\n");
            exit(0);
          }
        printf("New connection has arrived!\n");
        if(server_type ==1)
          {
            printf("Starting WEATHER worker thread for the client!\n");
            pthread_create(&worker_threads[client_count], &attr, weather_worker_thread_start, (void *)&new_connection_sd);
          }
        else if(server_type ==2)
          {
            printf("Starting STOCK worker thread for the client!\n");
            pthread_create(&worker_threads[client_count], &attr, stock_worker_thread_start, (void *)&new_connection_sd);
          }
        else
          {
            printf ("Unknown server type. Exiting program.\n");
            exit(0);
          }
        
        client_count++;
        
        if(client_count >= MAX_CLIENTS)
          {
            printf("Maximum client connections has reached! Exiting program.\n");
            exit(1);
          }
     }
}
/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

int find_temperature(char* city)
{
    for(int i = 0; i < cities_size; i++)
       {
         if(strcmp(city, cities[i]) == 0)
           {
             return temperatures[i];
           }
       }
    
    printf("Did not find the city %s in the database!", city);
    return -1;
}

/*-------------------------------------------------------------------------------------------------------------------------------------------------*/

float find_stock_price(char* stock)
{
    printf("STOCK: %s\n", stock);
    for(int i = 0; i < stocks_size; i++)
       {
         if(strcmp(stock, stocks[i]) == 0)
           {
             return stock_prices[i];
           }
        }
    
    printf("Did not find the stock %s in the database!", stock);
    return -1.0;
}

/* ------------------------------------------------------------------------------------------------------------------------------------------------*/

void* weather_worker_thread_start(void* sd)
{
    char city[50] = {0};
    char client_name[50] = {0};
    char server_response[1024] = {0};
    int bytes_read;
    char read_buffer[READ_BUFFER_SIZE] = {0};                   /* this array is to read the data from client */
    int socket_sd;
    int * temp;
    
    printf("Waiting for message from client ... \n");
    temp = (int *)sd;
    socket_sd = *temp;

    /* Start READING bytes from client */
    while((bytes_read = read(socket_sd, read_buffer, READ_BUFFER_SIZE)) > 0 )
    {
        read_buffer[bytes_read] = 0;                            /* make it as a string by adding 0 at the end of number of bytes read */
        printf("Bytes sent by client: %s \n", read_buffer);
        
        /*Start SENDING same bytes to client */
        /*strcpy(city, read_buffer);*/
        
        sscanf(read_buffer,"%s %s\n", client_name, city);
       
        int temperature = find_temperature(city);
        if(temperature != -1)
          {
            sprintf(server_response, "The temperature of %s is: %d F\n", city, temperature);
          }
        else
          {
            sprintf(server_response, "This is the WEATHER SERVER. Please check your port %d to talk to WEATHER server or you sent an unknown city: %s\n", port, city);
          }
        
        printf("Sending to client %s: %s\n",client_name,server_response);
        send(socket_sd, server_response, strlen(server_response),0);
    }
    return NULL;
}

/*----------------------------------------------------------------------------------------------------------------------------------------------------*/

void* stock_worker_thread_start(void* sd)
{
    char stock_name[50] = {0};
    char client_name[50] = {0};
    char server_response[1024] = {0};
    int bytes_read;
    char read_buffer[READ_BUFFER_SIZE] = {0};                       /* this array is to read the data from client */
    int socket_sd;
    int * temp;
    
    printf("Waiting for message from client ... \n");
    temp = (int *)sd;
    socket_sd = *temp;

    /* Start READING bytes from client */
    while((bytes_read = read(socket_sd, read_buffer, READ_BUFFER_SIZE)) > 0 )
         {
            read_buffer[bytes_read] = 0;                            /* make it as a string by adding 0 at the end of number of bytes read */
            printf("Bytes sent by client: %s \n", read_buffer);
            sscanf(read_buffer,"%s %s\n", client_name, stock_name);
            printf("STOCK_NAME: %s\n", stock_name);
            printf("CLIENT_NAME: %s\n", client_name);
            float price = find_stock_price(stock_name);
        
            if(price != -1.0)
              {
                printf("BASE PRICE: %.2f", price);
                srand(time(0));                     /* initialize rand with seed */
                int randnum1 = rand();
                printf("RANDNUM: %d\n", randnum1);
                float randnum = (randnum1 % 100) / 100.0;
                printf("RANDOM NUM: %f ", randnum);
                if(randnum1 % 2 == 0)
                  {
                    price = price + randnum;
                  }
                else
                  {
                    price = price - randnum;
                  }
                sprintf(server_response, "The stock price of %s is: $ %.2f \n", stock_name, price);
              }
            else
              {
                sprintf(server_response, "This is the STOCK SERVER. Please start STOCK CLIENT with option 'S' and with port %d to talk to STOCK server or you sent an unknown stock: %s\n", port, stock_name);
              }
        
            /* printf("Sending same message back to client!\n");
            send(new_connection_sd, read_buffer, strlen(read_buffer),0); -- echo line */
        
            printf("Sending to client %s: %s\n",client_name,server_response);
            send(socket_sd, server_response, strlen(server_response),0);
         }
    return NULL;
}
