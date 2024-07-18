#include "../include/jogo.h"

void initialize_board(char board[][3]) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) board[i][j] = ' ';
    }
}

void draw_instructions() {
    printf(" --- Jogo da Velha --- \n");
    printf(" 1 | 2 | 3 \n");
    printf("---|---|---\n");
    printf(" 4 | 5 | 6 \n");
    printf("---|---|---\n");
    printf(" 7 | 8 | 9 \n");
    printf(
        "Digite um número de 1 a 9 para fazer um movimento, conforme o "
        "tabuleiro acima.\n\n");
}

void draw_board(char board[][3]) {
    printf(" %c | %c | %c \n", board[0][0], board[0][1], board[0][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[1][0], board[1][1], board[1][2]);
    printf("---|---|---\n");
    printf(" %c | %c | %c \n", board[2][0], board[2][1], board[2][2]);
}

void change_player(char* player) {
    if (*player == 'X') {
        *player = 'O';
    } else {
        *player = 'X';
    }
}

int make_move(char board[][3], char player, int move) {
    int x, y;
    x = (move - 1) / 3;
    y = (move - 1) % 3;

    if (board[x][y] == ' ') {
        board[x][y] = player;
        return 1;
    } else {
        return 0;
    }
}

int check_row(char board[][3]) {
    for (int i = 0; i < 3; i++) {
        if ((board[i][0] == board[i][1]) && (board[i][0] == board[i][2]) &&
            (board[i][0] != ' '))
            return 1;
    }
    return 0;
}

int check_column(char board[][3]) {
    for (int i = 0; i < 3; i++) {
        if ((board[0][i] == board[1][i]) && (board[0][i] == board[2][i]) &&
            (board[0][i] != ' '))
            return 1;
    }
    return 0;
}

int check_diagonal(char board[][3]) {
    if ((board[0][0] == board[1][1]) && (board[0][0] == board[2][2]) &&
        (board[0][0] != ' '))
        return 1;
    if ((board[0][2] == board[1][1]) && (board[0][2] == board[2][0]) &&
        (board[0][2] != ' '))
        return 1;
    return 0;
}

int check_win(char board[][3]) {
    if (check_column(board) || check_row(board) || check_diagonal(board))
        return 1;
    return 0;
}

void run_game() {
    char board[3][3];
    char player = 'X';

    draw_instructions();
    initialize_board(board);
    draw_board(board);
    for (int i = 0; i < 9; i++) {
        int move;
        printf("Movimento:\n");
        scanf("%d", &move);
        int val_move = make_move(board, player, move);
        draw_board(board);
        if (val_move == 0) {
            i--;
            continue;
        }
        if (check_win(board) == 1) {
            printf("Fim de jogo!\n");
            printf("Vitoria de %c!\n", player);
            break;
        }
        printf("\n");
        change_player(&player);
    }
}

int main() {
    run_game();

    return 1;
}