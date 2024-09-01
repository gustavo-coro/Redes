#include "../include/thread_server.h"

void *thread_connection_handler(void *args) {
    int cli_sockfd = *((int *)args);
    free(args);

    connection_handler(cli_sockfd);

    pthread_exit(NULL);
}

int server_with_threads(int sockfd) {
    int r;
    printf("--- THREAD SERVER ---\n");

    while (1) {
        printf("waiting for connections...\n");

        int *cli_sockfd = (int *)malloc(sizeof(int));
        *cli_sockfd = accept_connection(sockfd);

        if (*cli_sockfd >= 0) {
            printf("\nconnection made with client %d\n", *cli_sockfd);

            pthread_t thread;
            r = pthread_create(&thread, NULL, thread_connection_handler,
                               cli_sockfd);
            pthread_detach(thread);

            if (r != 0) {
                fprintf(stderr, "%d: ", r);
                perror("ERROR on pthread_create\n");
            }
        } else {
            free(cli_sockfd);
        }
    }

    return 0;
}