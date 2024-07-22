#include "../include/cliente.h"

#include "../include/yatzy.h"

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

void receive_ints(int *clientSocket, int message[5]) {
    int n = read(*clientSocket, message, sizeof(int) * 5);
    if (n < 0 || n != (sizeof(int) * 5))
        error("Erro recebendo inteiro do servidor");
}

void send_int(int *clientSocket, int message) {
    int n = write(*clientSocket, &message, sizeof(int));
    if (n < 0) error("Erro enviando inteiro ao servidor");
}

void send_ints(int *clientSocket, int message[5]) {
    int n = write(*clientSocket, message, sizeof(int) * 5);
    if (n < 0) error("Erro enviando inteiros ao servidor");
}

void choose_category(int *clientSocket) {
    char buffer[5];
    while (1) {
        printf("Escolha a categoria (1 - 13): ");
        fgets(buffer, 5, stdin);
        int category = atoi(buffer);
        if (category <= 13 && category >= 1) {
            printf("\n");
            send_int(clientSocket, category);
            break;
        } else {
            printf("Opcao invalida! Escolha novamente.\n ");
        }
    }
}

void take_turn(int *clientSocket, int *player, int id) {
    printf("Turno do jogador %d:\n", id + 1);
    int dice[NUM_DICE] = {0};

    for (int i = 3; i > 0; i--) {
        printf("Rolar Dados (%d)\n", i);
        send_ints(clientSocket, dice);
        receive_ints(clientSocket, dice);
        printDice(dice);

        if (i > 1) {  // No need to keep slots on the last roll
            int slots[NUM_DICE];
            int valid_input = 0;
            while (valid_input == 0) {
                printf(
                    "Escolha as posicoes que deseja salvar (1-5, separado por espaco):\n");

                for (int j = 0; j < NUM_DICE; j++) {
                    slots[j] = 0;
                }
                char input[MAX_INPUT];
                fgets(input, MAX_INPUT, stdin);

                char *token = strtok(input, " ");
                valid_input = 1;
                while (token != NULL) {
                    int slot = atoi(token);
                    if (slot >= 1 && slot <= 5) {
                        slots[slot - 1] = 1;  // Mark the chosen slots
                    } else {
                        valid_input = 0;
                        printf(
                            "Numero de posicao invalido! Escolha entre 1 e 5.\n");
                    }
                    token = strtok(NULL, " ");
                }
            }

            for (int j = 0; j < NUM_DICE; j++) {
                if (slots[j] == 0) {
                    dice[j] = 0;
                }
            }
        }
    }
}

void receive_update(int *clientSocket, int *player1, int *player2) {
    int number_updates = receive_int(clientSocket);
    for (int i = 0; i < number_updates; i++) {
        int player_id = receive_int(clientSocket);
        int table_position = receive_int(clientSocket);
        int score = receive_int(clientSocket);
        if (player_id == 0) {
            player1[table_position] = score;
        } else if (player_id == 1) {
            player2[table_position] = score;
        } else {
            printf("Erro ao receber id do servidor.\n");
        }
    }
}

int main(int argc, char **argv) {
    int clientSocket, portno;
    struct sockaddr_in serverAddr;
    char buffer[4];
    int *player1;
    int *player2;

    if (argc < 2) {
        error("ERRO, numero de porta nao providenciado!");
    }

    portno = atoi(argv[1]);

    make_connection(&clientSocket, portno, &serverAddr);
    int id = receive_int(&clientSocket);

    do {
        receive_message(&clientSocket, buffer);
        if (!strcmp(buffer, "HLD"))
            printf("Esperando pelo segundo jogador...\n");
    } while (strcmp(buffer, "SRT"));

    printf("O Jogo Comecou!\n");
    printInstructions();
    player1 = initializeTable();
    player2 = initializeTable();
    printf("Voce esta jogando como Jogador %d\n", id + 1);

    printYatzyTable(player1, player2);

    while (1) {
        receive_message(&clientSocket, buffer);

        if (!strcmp(buffer, "TRN")) {
            if (id == 0) {
                take_turn(&clientSocket, player1, id);
            } else {
                take_turn(&clientSocket, player2, id);
            }
        } else if (!strcmp(buffer, "INV")) {
            printf("Posicao ja preenchida. Tente outra vez.\n");
        } else if (!strcmp(buffer, "CAT")) {
            choose_category(&clientSocket);
        } else if (!strcmp(buffer, "UPD")) {
            receive_update(&clientSocket, player1, player2);
            printYatzyTable(player1, player2);
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
    freeTable(player1, player2);
    printf("Conexao encerrada.\n");
    return 1;
}
