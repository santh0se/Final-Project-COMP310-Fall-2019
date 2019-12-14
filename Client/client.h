#ifndef CLIENT_H
#define CLIENT_H
#include <stdio.h>
#include <stdlib.h>
#define SERVER_PORT 1027
/*#define SERVER_PORT 80*/
#define READ_BUFFER_SIZE 1024
#define LOCAL_SERVER_IP "127.0.0.1"
/*#define LOCAL_SERVER_IP "172.217.6.110" -- google.com */

/*-------------- API | GLOBAL METHODS-------------------------*/

void display_weather_menu();
void display_stock_menu();

#endif
