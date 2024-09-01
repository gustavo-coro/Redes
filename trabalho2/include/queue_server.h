#ifndef QUEUE_SERVER_H
#define QUEUE_SERVER_H

#include <pthread.h>
#include "server.h"

#define NUM_THREADS 4

void *queue_thread_connection_handler(void *);
int server_with_queue(int);

#endif