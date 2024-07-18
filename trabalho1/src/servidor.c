#include "../include/servidor.h"

void error(const char* msg) {
    perror(msg);
    exit(1);
}

void start_socket(int* sockfd, int portno, struct sockaddr_in* serverAddr,
                  int* clientSockets, int maxClients) {
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) error("ERRO ao abrir o socket");

    for (int i = 0; i < maxClients; i++) {
        clientSockets[i] = 0;
    }

    int opt = 1;
    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                   sizeof(opt)) < 0) {
        error("setsockopt");
    }

    printf("Socket do Server Criado com Sucesso!\n");
    memset(serverAddr, '\0', sizeof(*serverAddr));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_addr.s_addr = INADDR_ANY;
    serverAddr->sin_port = htons(portno);

    if (bind(*sockfd, (struct sockaddr*)serverAddr, sizeof(*serverAddr)) < 0) {
        error("ERRO no binding");
    }

    printf("Bind na Porta numero %d.\n", portno);

    if (listen(*sockfd, maxClients) < 0) {
        error("ERRO no listening");
    }

    printf("Listening...\n");
}

void make_connections(socklen_t* cli_len, int* sockfd, int* newSocket,
                      struct sockaddr_in* cliAddr, fd_set* readFds,
                      int* clientSockets, int* sd, int maxClients) {
    FD_ZERO(readFds);
    FD_SET(*sockfd, readFds);
    int max_sd = *sockfd;

    for (int i = 0; i < maxClients; i++) {
        *sd = clientSockets[i];

        if (*sd > 0) FD_SET(*sd, readFds);

        if (*sd > max_sd) max_sd = *sd;
    }

    int activity = select(max_sd + 1, readFds, NULL, NULL, NULL);
    if ((activity < 0) && (errno != EINTR)) {
        printf("erro no select");
    }

    if (FD_ISSET(*sockfd, readFds)) {
        *cli_len = sizeof(*cliAddr);
        if ((*newSocket = accept(*sockfd, (struct sockaddr*)cliAddr, cli_len)) <
            0) {
            error("accept");
        }

        printf("Nova conexao, socket fd e %d, ip e : %s, porta : %d\n",
               *newSocket, inet_ntoa(cliAddr->sin_addr),
               ntohs(cliAddr->sin_port));

        for (int i = 0; i < maxClients; i++) {
            if (clientSockets[i] == 0) {
                clientSockets[i] = *newSocket;
                printf("Adicionando para a lista de sockets como %d\n", i);
                break;
            }
        }
    }
}

void talk_to_client(char buffer[2048], fd_set* readFds, int* sd,
                    int* clientSockets, struct sockaddr_in* cliAddr,
                    socklen_t* cli_len, int maxClients) {
    for (int i = 0; i < maxClients; i++) {
        *sd = clientSockets[i];

        if (FD_ISSET(*sd, readFds)) {
            int valread = read(*sd, buffer, 2048);
            if (valread == 0) {
                getpeername(*sd, (struct sockaddr*)cliAddr, cli_len);
                printf("Host desconectado, ip %s, porta %d\n",
                       inet_ntoa(cliAddr->sin_addr), ntohs(cliAddr->sin_port));
                close(*sd);
                clientSockets[i] = 0;
            } else {
                buffer[valread] = '\0';
                printf("Mensagem recebida: %s\n", buffer);
                send(*sd, buffer, strlen(buffer), 0);
            }
        }
    }
}

int main(int argc, char** argv) {
    char buffer[2048];
    int sockfd, newSocket, portno, maxClients = 2, sd;
    int clientSockets[maxClients];
    socklen_t cli_len;
    struct sockaddr_in serverAddr, cliAddr;
    fd_set readFds;

    if (argc < 2) {
        fprintf(stderr, "ERRO! porta nao providenciada.\n");
        exit(1);
    }

    portno = atoi(argv[1]);

    start_socket(&sockfd, portno, &serverAddr, clientSockets, maxClients);

    while (1) {
        make_connections(&cli_len, &sockfd, &newSocket, &cliAddr, &readFds,
                         clientSockets, &sd, maxClients);

        talk_to_client(buffer, &readFds, &sd, clientSockets,
                       &cliAddr, &cli_len, maxClients);
    }

    close(sockfd);
    return 1;
}
