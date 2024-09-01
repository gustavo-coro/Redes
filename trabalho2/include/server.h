#ifndef SERVER_H
#define SERVER_H

#include <arpa/inet.h>
#include <netinet/in.h>
#include <errno.h>
#include <fcntl.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/types.h>
#include <sys/sendfile.h>

#include <features.h>
#include <ifaddrs.h>

#define PORT 8080
#define BUFFSIZE 2048
#define MAXPENDING 5
#define MAX_PATH_LEN pathconf(getcwd(NULL, 0), _PC_PATH_MAX)

void error(const char *);
int start_socket(int, int);
int accept_connection(int);
void connection_handler(int);
void send_response(int, const char *, const char *, const char *, size_t);
char *return_mime(const char *);
void request_handler(int, const char *);

#endif