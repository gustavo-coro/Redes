#ifndef CLIENTE_H
#define CLIENTE_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error(const char *);
void make_connection(int *, int, struct sockaddr_in *);
void receive_message(int*, char*);
int receive_int(int*);
void send_int(int*, int);
void make_a_move(int*);
void receive_update(int*, char*);


#endif