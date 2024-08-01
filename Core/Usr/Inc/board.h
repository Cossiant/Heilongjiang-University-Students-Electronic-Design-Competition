#ifndef _BOARD_H_
#define _BOARD_H_

#include <stdio.h>
#include <stdint.h>
#include "alpha_beta.h"
#include "usart.h"
#include "data.h"
#include "Robot.h"

typedef struct {
    int cnt, player, computer, turn, end, id1, id2;
    uint8_t recv, WAKE, send, buffer[10];
    int old_board[3][3];
    int new_board[3][3];
} Board;

extern Board opeBoard;

void Board_init(Board *board, int player, int computer) ;

void Board_set(Board *board, int player, int computer);

void Board_next_turn(Board *board);

void Board_update_board(Board *board);

int Board_moveBoard(Board *board, int id, ROBOT_DATA *data);

int Board_currentMove(Board *board, int x, int y, ROBOT_DATA *data);

int Board_check(Board *board, int *x, int *y);

void Board_receiveBoard(Board *board, UART_HandleTypeDef *huart, ROBOT_DATA *data);

int Board_play(Board *board, UART_HandleTypeDef *huart, ROBOT_DATA *data, KEY_DATA *Key);


#endif // _BOARD_H_
