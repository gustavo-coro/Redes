#include "../include/concurrent_server.h"

int concurrent_server(int sockfd) {
    printf("--- CONCURRENT SERVER ---\n");
    printf("waiting for connections...\n");

    fd_set master;
    fd_set read_fds;
    int fdmax;
    int newfd;
    int i;

    FD_ZERO(&master);
    FD_ZERO(&read_fds);

    FD_SET(sockfd, &master);

    fdmax = sockfd;

    while (1) {
        read_fds = master;

        if (select(fdmax + 1, &read_fds, NULL, NULL, NULL) == -1) {
            error("ERROR on server-select()!");
        }
        printf("Server-select() is OK...\n");

        for (i = 0; i <= fdmax; i++) {
            if (FD_ISSET(i, &read_fds)) {
                if (i == sockfd) {
                    newfd = accept_connection(sockfd);

                    if (newfd != -1) {
                        FD_SET(newfd, &master);
                        if (newfd > fdmax) {
                            fdmax = newfd;
                        }
                        printf("\nconnection made with client %d\n", newfd);
                    }
                } else {
                    printf("\nhandling %d\n", i);
                    connection_handler(i);

                    FD_CLR(i, &master);
                }
            }
        }
    }

    return 0;
}