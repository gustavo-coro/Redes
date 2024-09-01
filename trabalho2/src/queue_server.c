#include "../include/queue_server.h"

int max_queue_size = 256;
int *queue = NULL;
int queue_len = 0;
pthread_t *queue_threads;

pthread_mutex_t queue_mutex = PTHREAD_MUTEX_INITIALIZER;
pthread_cond_t queue_cond = PTHREAD_COND_INITIALIZER;

void *queue_thread_connection_handler(void *args) {
    int cli_sokckfd;
    while (1) {
        pthread_mutex_lock(&queue_mutex);
        while (queue_len == 0) {
            pthread_cond_wait(&queue_cond, &queue_mutex);
        }
        cli_sokckfd = queue[--queue_len];
        if (queue_len == max_queue_size - 1) {
            pthread_cond_signal(&queue_cond);
        }
        pthread_mutex_unlock(&queue_mutex);

        connection_handler(cli_sokckfd);
    }
}

int server_with_queue(int sockfd) {
    int cli_sockfd;

    printf("--- QUEUE SERVER ---\n");

    queue_threads = malloc(sizeof(pthread_t) * NUM_THREADS);
    if (queue_threads == NULL) {
        error("ERROR: Memory allocation for queue_threads failed");
    }

    for (int i = 0; i < NUM_THREADS; i++) {
        if (pthread_create(&queue_threads[i], NULL,
                           queue_thread_connection_handler, NULL) != 0) {
            free(queue_threads);
            error("ERROR on thread creation");
        }
    }

    queue = malloc(max_queue_size * sizeof(int));

    while (1) {
        printf("waiting for connections...\n");

        int cli_sockfd = accept_connection(sockfd);
        if (cli_sockfd >= 0) {
            pthread_mutex_lock(&queue_mutex);
            while (queue_len == max_queue_size) {
                pthread_cond_wait(&queue_cond, &queue_mutex);
            }
            queue[queue_len] = cli_sockfd;
            queue_len++;
            if (queue_len == 1) {
                pthread_cond_signal(&queue_cond);
            }
            pthread_mutex_unlock(&queue_mutex);
        }
    }
    for (int i = 0; i < NUM_THREADS; i++) {
        pthread_join(queue_threads[i], NULL);
    }

    return 0;
}