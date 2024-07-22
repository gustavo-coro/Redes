#ifndef CLIENTE_H
#define CLIENTE_H

#define MAX_INPUT 15

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error(const char *);
void make_connection(int *, int, struct sockaddr_in *);
void receive_message(int *, char *);
int receive_int(int *);
void receive_ints(int *, int[5]);
void send_int(int *, int);
void send_ints(int *, int[5]);
void choose_category(int *);
void take_turn(int *, int *, int);
void receive_update(int *, int *, int *);

#endif