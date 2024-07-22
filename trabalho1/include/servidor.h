#ifndef SERVIDOR_H
#define SERVIDOR_H

#include <arpa/inet.h>
#include <errno.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <sys/types.h>
#include <unistd.h>

void error(const char*);
void start_socket(int*, int, struct sockaddr_in*);
void send_buffer(int, const void*, size_t);
void send_buffer_to_all_clients(int*, const void*, size_t);
int receive_int(int);
void receive_ints(int, int[5]);
void make_connections(int*, int*, struct sockaddr_in*, socklen_t*);
int* send_player_dices(int);
int get_category(int);
void send_update(int*, int, int, int);
void send_updated_table(int*, int*, int, int);
void* run_game(void*);

#endif