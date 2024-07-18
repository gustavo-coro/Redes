#include "../include/cliente.h"

void error(const char *msg) {
    perror(msg);
    exit(1);
}

void make_connection(int *clientSocket, int portno,
                     struct sockaddr_in *serverAddr) {
    *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*clientSocket < 0) {
        error("Erro no socket");
    }
    printf("Socket do Cliente foi Criado!\n");

    memset(serverAddr, '\0', sizeof(*serverAddr));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htons(portno);
    serverAddr->sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(*clientSocket, (struct sockaddr *)serverAddr,
                sizeof(*serverAddr)) < 0) {
        perror("Erro ao conectar");
        exit(1);
    }
    printf("Conectado ao Servidor!\n");
}

void talk_to_server(char buffer[2048], int *clientSocket) {
    while (1) {
        printf("Entre a mensagem: ");
        fgets(buffer, 2048, stdin);
        buffer[strcspn(buffer, "\n")] = '\0';

        send(*clientSocket, buffer, strlen(buffer), 0);

        if (strcmp(buffer, "sair") == 0) {
            break;
        }

        int n = recv(*clientSocket, buffer, 2048, 0);
        if (n < 0) {
            error("Erro recebendo os dados");
        }
        buffer[n] = '\0';
        printf("Dados recebidos: %s\n", buffer);
    }
}

int main(int argc, char **argv) {
    int clientSocket, portno;
    struct sockaddr_in serverAddr;
    char buffer[2048];

    if (argc < 2) {
        error("ERRO, numero de porta nao providenciado!");
    }

    portno = atoi(argv[1]);

    make_connection(&clientSocket, portno, &serverAddr);
    talk_to_server(buffer, &clientSocket);

    close(clientSocket);
    printf("Conexao encerrada.\n");
    return 1;
}
