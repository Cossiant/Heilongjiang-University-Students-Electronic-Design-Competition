#ifndef _ALPHA_BETA_H
#define _ALPHA_BETA_H
#define INT_MIN -1000000
#define INT_MAX 1000000

#ifndef BLACK
#define BLACK 1
#define WHITE 2
#endif

#include <stdio.h>
#include <stdlib.h>
#include <limits.h>


int evaluate(int board[3][3], int computer);

int alphaBeta(int board[3][3], int depth, int alpha, int beta, int isMaximizing, int cnt, int computer);

void findBestMove(int board[3][3], int *row, int *col, int cnt, int computer);

void printBoard(int board[3][3]);

int count(int board[3][3]);

#endif
