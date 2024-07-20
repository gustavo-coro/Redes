#include "../include/servidor.h"

#include "../include/jogo.h"

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
        printf("Numero de jogadores conectados atualizado: %d.\n", playersConnected);
        pthread_mutex_unlock(&mutexCount);

        if (connections == 0) {
            send_buffer(clientSockets[0], "HLD", strlen("HLD"));
        }
        connections++;
    }
}

int get_player_move(int clientSocket) {
    send_buffer(clientSocket, "TRN", strlen("TRN"));

    return receive_int(clientSocket);
}

void send_update(int* clientSockets, int move, int id) {
    send_buffer_to_all_clients(clientSockets, "UPD", strlen("UPD"));
    send_buffer_to_all_clients(clientSockets, &id, sizeof(int));
    send_buffer_to_all_clients(clientSockets, &move, sizeof(int));
}

void* run_game(void* thread_data) {
    int* clientSockets = (int*)thread_data;
    char board[9];

    send_buffer_to_all_clients(clientSockets, "SRT", strlen("SRT"));

    initialize_board(board);

    int prev_player_turn = 1;
    int player_turn = 0;
    int game_over = 0;
    int turn_count = 0;
    while (!game_over) {
        if (prev_player_turn != player_turn) {
            send_buffer(clientSockets[(player_turn + 1) % 2], "WAT",
                        strlen("WAT"));
        }

        int valid = 0;
        int move = 0;
        while (!valid) {
            move = get_player_move(clientSockets[player_turn]);
            if (move == -1) break;

            valid = make_move(board, player_turn ? 'O' : 'X', move);
            if (!valid) {
                send_buffer(clientSockets[(player_turn + 1) % 2], "INV",
                            strlen("INV"));
            }
        }

        if (move == -1) {
            break;
        } else {
            send_update(clientSockets, move, player_turn);

            game_over = check_win(board);

            if (game_over == 1) {
                send_buffer(clientSockets[player_turn], "WIN", strlen("WIN"));
                send_buffer(clientSockets[(player_turn + 1) % 2], "LSE",
                            strlen("LSE"));
            } else if (turn_count == 8) {
                send_buffer_to_all_clients(clientSockets, "DRW", strlen("DRW"));
                game_over = 1;
            }

            prev_player_turn = player_turn;
            player_turn = (player_turn + 1) % 2;
            turn_count++;
        }
    }

    close(clientSockets[0]);
    close(clientSockets[1]);

    pthread_mutex_lock(&mutexCount);
    playersConnected -= 2;
    pthread_mutex_unlock(&mutexCount);
    free(clientSockets);

    printf("Numero de jogadores conectados atualizado: %d.\n", playersConnected);

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
