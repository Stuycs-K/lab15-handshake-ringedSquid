#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>

#ifndef NETWORKING_H
#define NETWORKING_H
#define WKP "mario"

#define HANDSHAKE_BUFFER_SIZE 10
#define BUFFER_SIZE 1000
#define MAX_CLIENTS 100

#define SYN 0
#define SYN_ACK 1
#define ACK 2
#define MESSAGE 3
#define EXIT 4

#define DEBUG 1

int server_handshake(int *to_client);
int client_handshake(int *to_server);

//for forking server
int server_setup();
int server_handshake_half(int *to_client, int from_client);

#endif
