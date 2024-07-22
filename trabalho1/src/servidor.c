#include "../include/servidor.h"

#include "../include/yatzy.h"

int playersConnected = 0;
pthread_mutex_t mutexCount;

void error(const char* message) {
    perror(message);
    pthread_exit(NULL);
}

void start_socket(int* sockfd, int portno, struct sockaddr_in* serverAddr) {
    *sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (*sockfd < 0) error("ERRO ao abrir o socket");
    int opt = 1;
    if (setsockopt(*sockfd, SOL_SOCKET, SO_REUSEADDR, (char*)&opt,
                   sizeof(opt)) < 0) {
        error("Erro no setsockopt");
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
}

void send_buffer(int clientSocket, const void* buffer, size_t len) {
    int n = write(clientSocket, buffer, len);
    if (n < 0) error("Erro ao mandar mensagem ao cliente");
}

void send_buffer_to_all_clients(int* clientSockets, const void* buffer,
                                size_t len) {
    send_buffer(clientSockets[0], buffer, len);
    send_buffer(clientSockets[1], buffer, len);
}

int receive_int(int clientSocket) {
    int message = 0;
    int n = read(clientSocket, &message, sizeof(int));

    if (n < 0 || n != sizeof(int)) return -1;
    return message;
}

void receive_ints(int clientSocket, int message[5]) {
    int n = read(clientSocket, message, sizeof(int) * 5);

    if (n < 0 || n != (sizeof(int) * 5))
        error("Erro recebendo inteiro do cliente");
}

void make_connections(int* sockfd, int* clientSockets,
                      struct sockaddr_in* cliAddr, socklen_t* cliLen) {
    int connections = 0;
    while (connections < 2) {
        listen(*sockfd, 2);
        memset(cliAddr, 0, sizeof(*clientSockets));

        *cliLen = sizeof(*cliAddr);
        clientSockets[connections] =
            accept(*sockfd, (struct sockaddr*)cliAddr, cliLen);
        if (clientSockets[connections] < 0) error("Erro ao aceitar conexao");

        write(clientSockets[connections], &connections, sizeof(int));

        pthread_mutex_lock(&mutexCount);
        playersConnected++;
        printf("Numero de jogadores conectados atualizado: %d.\n",
               playersConnected);
        pthread_mutex_unlock(&mutexCount);

        if (connections == 0) {
            send_buffer(clientSockets[0], "HLD", strlen("HLD"));
        }
        connections++;
    }
}

int* send_player_dices(int clientSocket) {
    send_buffer(clientSocket, "TRN", strlen("TRN"));
    int* dice = (int*)malloc(sizeof(int) * NUM_DICE);
    for (int i = 0; i < 3; i++) {
        receive_ints(clientSocket, dice);
        rollDice(dice);
        send_buffer(clientSocket, dice, sizeof(int) * 5);
    }
    return dice;
}

int get_category(int clientSocket) {
    send_buffer(clientSocket, "CAT", strlen("CAT"));

    return receive_int(clientSocket);
}

void send_update(int* clientSockets, int position, int id, int score) {
    send_buffer_to_all_clients(clientSockets, &id, sizeof(int));
    send_buffer_to_all_clients(clientSockets, &position, sizeof(int));
    send_buffer_to_all_clients(clientSockets, &score, sizeof(int));
}

void send_updated_table(int* clientSockets, int* player, int category, int id) {
    send_buffer_to_all_clients(clientSockets, "UPD", strlen("UPD"));
    int number_att = 2;
    if (category >= 1 && category <= 6) {
        number_att = 4;
        send_buffer_to_all_clients(clientSockets, &number_att, sizeof(int));
        send_update(clientSockets, category - 1, id, player[category - 1]);
        send_update(clientSockets, 6, id, player[6]);
        send_update(clientSockets, 7, id, player[7]);
        send_update(clientSockets, 15, id, player[15]);
        return;
    }
    send_buffer_to_all_clients(clientSockets, &number_att, sizeof(int));
    send_update(clientSockets, category + 1, id, player[category + 1]);
    send_update(clientSockets, 15, id, player[15]);
}

void* run_game(void* thread_data) {
    int* clientSockets = (int*)thread_data;
    int* player1;
    int* player2;

    send_buffer_to_all_clients(clientSockets, "SRT", strlen("SRT"));

    player1 = initializeTable();
    player2 = initializeTable();

    int prev_player_turn = 1;
    int player_turn = 0;
    int turn_count = 0;
    while (turn_count < 26) {
        if (prev_player_turn != player_turn) {
            send_buffer(clientSockets[(player_turn + 1) % 2], "WAT",
                        strlen("WAT"));
        }

        int valid = 0;
        int category = 0;
        int* dice = send_player_dices(clientSockets[player_turn]);
        while (!valid) {
            category = get_category(clientSockets[player_turn]);
            if (category == -1) break;
            if (player_turn == 0) {
                valid = makeMove(player1, dice, category);
            } else {
                valid = makeMove(player2, dice, category);
            }

            if (!valid) {
                send_buffer(clientSockets[player_turn], "INV", strlen("INV"));
            }
        }
        free(dice);

        if (category == -1) {
            break;
        } else {
            if (player_turn == 0) {
                send_updated_table(clientSockets, player1, category, 0);
            } else {
                send_updated_table(clientSockets, player2, category, 1);
            }

            prev_player_turn = player_turn;
            player_turn = (player_turn + 1) % 2;
            turn_count++;
        }
    }

    if (player1[15] > player2[15]) {
        send_buffer(clientSockets[0], "WIN", strlen("WIN"));
        send_buffer(clientSockets[1], "LSE", strlen("LSE"));
    } else if (player1[15] < player2[15]) {
        send_buffer(clientSockets[1], "WIN", strlen("WIN"));
        send_buffer(clientSockets[0], "LSE", strlen("LSE"));
    } else {
        send_buffer_to_all_clients(clientSockets, "DRW", strlen("DRW"));
    }

    close(clientSockets[0]);
    close(clientSockets[1]);

    pthread_mutex_lock(&mutexCount);
    playersConnected -= 2;
    pthread_mutex_unlock(&mutexCount);
    free(clientSockets);
    freeTable(player1, player2);

    printf("Numero de jogadores conectados atualizado: %d.\n",
           playersConnected);

    pthread_exit(NULL);
}

int main(int argc, char** argv) {
    if (argc < 2) {
        fprintf(stderr, "ERRO! porta nao providenciada.\n");
        exit(1);
    }

    int sockfd, newSocket, portno, sd;
    int* clientSockets;
    socklen_t cliLen;
    struct sockaddr_in serverAddr, cliAddr;
    fd_set readFds;

    portno = atoi(argv[1]);

    start_socket(&sockfd, portno, &serverAddr);

    pthread_mutex_init(&mutexCount, NULL);

    while (1) {
        if (playersConnected <= 252) {
            int* clientSockets = (int*)malloc(2 * sizeof(int));
            memset(clientSockets, 0, 2 * sizeof(int));

            make_connections(&sockfd, clientSockets, &cliAddr, &cliLen);

            pthread_t thread;
            int result =
                pthread_create(&thread, NULL, run_game, (void*)clientSockets);
            if (result) {
                printf("Criacao da thread falhou! Retorno: %d\n", result);
                exit(-1);
            }
        }
    }

    close(sockfd);
    pthread_mutex_destroy(&mutexCount);
    pthread_exit(NULL);
    return 1;
}
