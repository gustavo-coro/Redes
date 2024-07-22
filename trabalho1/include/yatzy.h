#ifndef YATZY_H
#define YATZY_H

#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define NUM_CATEGORIES 16
#define NUM_DICE 5

int* initializeTable();
void freeTable(int*, int*);
void printInstructions();
void printYatzyTable(int*, int*);
void printDice(int*);
void rollDice(int[NUM_DICE]);
int makeMove(int*, int*, int);

#endif