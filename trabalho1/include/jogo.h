#ifndef JOGO_H
#define JOGO_H

#include <stdio.h>
#include <stdlib.h>

void initialize_board(char*);
void draw_instructions();
void draw_board(char*);
int make_move(char*, char,
              int);  // return 0 in case the movement is invalid
int check_row(char*);
int check_column(char*);
int check_diagonal(char*);
int check_win(char*);

#endif