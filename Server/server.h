#ifndef SERVER_H
#define SERVER_H
#include <stdio.h>
#include <stdlib.h>
#define PORT 1025
#define CONNECTION_BACKLOG 10
#define READ_BUFFER_SIZE 1024


int find_temperature(char* city);
float find_stock_price(char* stock);
void* weather_worker_thread_start(void * new_connection_sd);
void* stock_worker_thread_start(void* sd);


pthread_t worker_threads[MAX_WORKER_THREADS];
pthread_attr_t attr;
pthread_mutex_t worker_mutex;



#endif
