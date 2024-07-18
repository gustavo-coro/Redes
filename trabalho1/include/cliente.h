#ifndef CLIENTE_H
#define CLIENTE_H

#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

void error(const char *);
void make_connection(int *, int, struct sockaddr_in *);
void talk_to_server(char[2048], int *);

#endif