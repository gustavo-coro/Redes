#ifndef JOGO_H
#define JOGO_H

#include <stdio.h>
#include <stdlib.h>

void initialize_board(char (*)[3]);
void draw_instructions();
void draw_board(char (*)[3]);
void change_player(char*);
int make_move(char (*)[3], char,
              int);  // return 0 in case the movement is invalid
int check_row(char (*)[3]);
int check_column(char (*)[3]);
int check_diagonal(char (*)[3]);
int check_win(char (*)[3]);
void run_game();

#endif