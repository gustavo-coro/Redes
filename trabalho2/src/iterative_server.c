#include "../include/iterative_server.h"

int iterative_server(int sockfd) {
    int cli_sockfd;

    while (1) {

		printf("--- ITERATIVE SERVER ---\n");
		printf("waiting for connections...\n");

		cli_sockfd = accept_connection(sockfd);

		if (cli_sockfd >= 0) {
			printf("\nconnection made with client %d\n", cli_sockfd);
		}

		connection_handler(cli_sockfd);

    }

    return 0;
}