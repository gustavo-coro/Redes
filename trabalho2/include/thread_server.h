#ifndef THREAD_SERVER_H
#define THREAD_SERVER_H

#include <pthread.h>
#include "server.h"

void *thread_connection_handler(void *);
int server_with_threads(int);

#endif