#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <arpa/inet.h>
#include <errno.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void start_socket(int*, int, struct sockaddr_in*, int*, int);
void make_connections(socklen_t*, int*, int*, struct sockaddr_in*, fd_set*,
                      int*, int*, int);
void talk_to_client(char[2048], fd_set*, int*, int*, struct sockaddr_in*,
                    socklen_t*, int);

#endif