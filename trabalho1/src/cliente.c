#include "../include/cliente.h"

#include "../include/jogo.h"

void error(const char *message) {
    perror(message);
    exit(1);
}

void make_connection(int *clientSocket, int portno,
                     struct sockaddr_in *serverAddr) {
    *clientSocket = socket(AF_INET, SOCK_STREAM, 0);
    if (*clientSocket < 0) {
        error("Erro no socket");
    }

    memset(serverAddr, '\0', sizeof(*serverAddr));
    serverAddr->sin_family = AF_INET;
    serverAddr->sin_port = htons(portno);
    serverAddr->sin_addr.s_addr = inet_addr("127.0.0.1");

    if (connect(*clientSocket, (struct sockaddr *)serverAddr,
                sizeof(*serverAddr)) < 0) {
        error("Erro ao conectar");
    }
    printf("Conectado ao Servidor!\n");
}

void receive_message(int *clientSocket, char *message) {
    memset(message, 0, 4);
    int n = read(*clientSocket, message, 3);

    if (n < 0 || n != 3) error("Erro recebendo os dados do servidor");
}

int receive_int(int *clientSocket) {
    int message = 0;
    int n = read(*clientSocket, &message, sizeof(int));

    if (n < 0 || n != sizeof(int)) error("Erro recebendo inteiro do servidor");

    return message;
}

void send_int(int *clientSocket, int message) {
    int n = write(*clientSocket, &message, sizeof(int));
    if (n < 0) error("Erro enviando inteiro ao servidor");
}

void make_a_move(int *clientSocket) {
    char buffer[5];
    while (1) {
        printf("Movimento (1 - 9): ");
        fgets(buffer, 5, stdin);
        int move = buffer[0] - '0';
        if (move <= 9 && move >= 0) {
            printf("\n");
            send_int(clientSocket, move);
            break;
        } else {
            printf("\nEscolha um numero valido!\n");
        }
    }
}

void receive_update(int *clientSocket, char board[9]) {
    int player_id = receive_int(clientSocket);
    int move = receive_int(clientSocket);
    board[move - 1] = player_id ? 'O' : 'X';
}

int main(int argc, char **argv) {
    int clientSocket, portno;
    struct sockaddr_in serverAddr;
    char board[9], buffer[4];
    ;

    if (argc < 2) {
        error("ERRO, numero de porta nao providenciado!");
    }

    portno = atoi(argv[1]);

    make_connection(&clientSocket, portno, &serverAddr);
    int id = receive_int(&clientSocket);

    do {
        receive_message(&clientSocket, buffer);
        if (!strcmp(buffer, "HLD"))
            printf("Esperando pelo Segundo Jogador...\n");
    } while (strcmp(buffer, "SRT"));

    printf("O Jogo Comecou!\n");
    draw_instructions();
    initialize_board(board);
    printf("Voce esta jogando com %c\n", id ? 'O' : 'X');

    draw_board(board);

    while (1) {
        receive_message(&clientSocket, buffer);

        if (!strcmp(buffer, "TRN")) {
            printf("Escolha um movimento...\n");
            make_a_move(&clientSocket);
        } else if (!strcmp(buffer, "INV")) {
            printf("Posicao ja preenchida. Tente outra vez.\n");
        } else if (!strcmp(buffer, "UPD")) {
            receive_update(&clientSocket, board);
            draw_board(board);
        } else if (!strcmp(buffer, "WAT")) {
            printf("Aguardando movimento do outro jogador...\n");
        } else if (!strcmp(buffer, "WIN")) {
            printf("Voce Ganhou!\n");
            break;
        } else if (!strcmp(buffer, "LSE")) {
            printf("Voce Perdeu.\n");
            break;
        } else if (!strcmp(buffer, "DRW")) {
            printf("Empate.\n");
            break;
        } else
            error("Mensagem desconhecida.");
    }

    printf("Fim de Jogo.\n");

    close(clientSocket);
    printf("Conexao encerrada.\n");
    return 1;
}
