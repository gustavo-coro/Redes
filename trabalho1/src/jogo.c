#include "../include/jogo.h"

void initialize_board(char *board) {
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }
}

void draw_instructions() {
    printf("\t\t   --- Jogo da Velha --- \n");
    printf("\t\t\t 1 | 2 | 3 \n");
    printf("\t\t\t---|---|---\n");
    printf("\t\t\t 4 | 5 | 6 \n");
    printf("\t\t\t---|---|---\n");
    printf("\t\t\t 7 | 8 | 9 \n");
    printf(
        "Digite um número de 1 a 9 para fazer um movimento, conforme o "
        "tabuleiro acima.\n\n");
}

void draw_board(char *board) {
    printf("\t\t\t %c | %c | %c \n", board[0], board[1], board[2]);
    printf("\t\t\t---|---|---\n");
    printf("\t\t\t %c | %c | %c \n", board[3], board[4], board[5]);
    printf("\t\t\t---|---|---\n");
    printf("\t\t\t %c | %c | %c \n", board[6], board[7], board[8]);
}

int make_move(char *board, char player, int move) {
    if (board[move - 1] == ' ') {
        board[move - 1] = player;
        return 1;
    } else {
        printf("Movimento invalido.\n");
        return 0;
    }
}

int check_row(char *board) {
    for (int i = 0; i <= 6; i+=3) {
        if ((board[i] == board[i + 1]) && (board[i] == board[i + 2]) &&
            (board[i] != ' '))
            return 1;
    }
    return 0;
}

int check_column(char *board) {
    for (int i = 0; i < 3; i++) {
        if ((board[i] == board[i + 3]) && (board[i] == board[i + 6]) &&
            (board[i] != ' '))
            return 1;
    }
    return 0;
}

int check_diagonal(char *board) {
    if ((board[0] == board[4]) && (board[0] == board[8]) && (board[0] != ' '))
        return 1;
    if ((board[2] == board[4]) && (board[2] == board[6]) && (board[2] != ' '))
        return 1;
    return 0;
}

int check_win(char *board) {
    if (check_column(board) || check_row(board) || check_diagonal(board))
        return 1;
    return 0;
}