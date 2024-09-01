#include "../include/concurrent_server.h"
#include "../include/iterative_server.h"
#include "../include/queue_server.h"
#include "../include/thread_server.h"

int (*server_type[])(int) = {iterative_server, server_with_threads,
                             server_with_queue, concurrent_server};

int main(int argc, char **argv) {
    int server, sockfd;

    server = atoi(argv[1]);

    if (server < 0 || server > 3) {
        error("ERROR! invalid server type!");
    }

    sockfd = start_socket(PORT, MAXPENDING);

    puts("listening in:");

    struct ifaddrs *ifaddr, *ifa;
    struct sockaddr_in *sa;
    char ip4[INET_ADDRSTRLEN];

    if (getifaddrs(&ifaddr) == -1) {
        perror("getifaddrs");
        exit(EXIT_FAILURE);
    }
    for (ifa = ifaddr; ifa != NULL; ifa = ifa->ifa_next) {
        if (ifa->ifa_addr == NULL)
            continue;

        if (ifa->ifa_addr->sa_family == AF_INET) {
            sa = (struct sockaddr_in *) ifa->ifa_addr;
            inet_ntop(AF_INET, &(sa->sin_addr), ip4, INET_ADDRSTRLEN);
            printf("%s: http://%s:%d/index.html\n", ifa->ifa_name, ip4, PORT);
        }
    }
    freeifaddrs(ifaddr);

    server_type[server](sockfd);

    return 0;
}