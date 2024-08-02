#include "board.h"
#include "key.h"
#include "oled.h"

Board opeBoard;
#ifndef BLACK
#define BLACK 1
#define WHITE 2
#endif

// 初始化 Board 结构体
void Board_init(Board *board, int player, int computer) {
    board->cnt = 0;
    board->player = player;
    board->computer = computer;
    board->turn = 1;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board->new_board[i][j] = 0;
            board->old_board[i][j] = 0;
        }
    }
}

// 设置玩家和计算机
void Board_set(Board *board, int player, int computer) {
    board->player = player;
    board->computer = computer;
}

// 交换轮次
void Board_next_turn(Board *board) { board->turn = 3 - board->turn; }

// 更新棋盘
void Board_update_board(Board *board) {
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            board->old_board[i][j] = board->new_board[i][j];
        }
    }
}

// 棋盘移动函数（需实现）
int Board_moveBoard(Board *board, int id, ROBOT_DATA *data) {
    // 实现棋盘移动逻辑
    if (board->turn == BLACK)
        Robot_move_point(data, 11);
    else
        Robot_move_point(data, 10);
    // 吸
    HAL_Delay(300);
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
    HAL_Delay(300);

    Robot_move_point(data, 12);
    HAL_Delay(300);

    Robot_move_point(data, id);
    // 放

    HAL_Delay(300);
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);

    HAL_Delay(300);
    Robot_move_point(data, 0);

    return 0;
}

// 当前移动函数（需实现）
int Board_currentMove(Board *board, int x, int y, ROBOT_DATA *data) {
	uint8_t t = y;

	        HAL_UART_Transmit(&huart1, &t, 1, 10);

	        t = x;

	        HAL_UART_Transmit(&huart1, &t, 1, 10);
    if (board->new_board[(x-1)/3][(x-1)%3] == 0) {
        // 先移动到y

        Robot_move_point(data, y);
        // 吸函数
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
        HAL_Delay(300);

        Robot_move_point(data, 12);
        HAL_Delay(300);

        Robot_move_point(data, x);

        HAL_Delay(300);
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);

        HAL_Delay(300);
        Robot_move_point(data, 0);
        // 放
    } else {
        // 先移动到x
        Robot_move_point(data, x);
        // 吸函数
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
        HAL_Delay(300);

        Robot_move_point(data, 12);
        HAL_Delay(300);
        Robot_move_point(data, y);
        // 放
        HAL_Delay(300);
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);

        HAL_Delay(300);
        Robot_move_point(data, 0);
    }
    return 0;
}

// 检查棋盘是否改变
int Board_check(Board *board, int *x, int *y) {
    int cnt = 0, flag = 0;
    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            if (board->old_board[i][j] &&
                board->old_board[i][j] != board->new_board[i][j]) {
                flag = board->old_board[i][j];
            }
        }
    }

    if(flag){
    	for(int i=0;i<3;i++){
    		for(int j=0;j<3;j++){
    			if(board->old_board[i][j] == flag && board->new_board[i][j] == 0 || !board->old_board[i][j] && board->new_board[i][j]==flag){
    				if(cnt++){
    					*y=i*3+j+1;
    				} else {
    					*x=i*3+j+1;
    				}
    			}
    		}
    	}
    }


    return flag;
}

// 接收棋盘数据
void Board_receiveBoard(Board *board, UART_HandleTypeDef *huart,
                        ROBOT_DATA *data) {
    board->send = (uint8_t)0xB1;
    HAL_UART_Transmit(huart, &board->send, 1, 10);
//    sum++;
//    HAL_UART_Transmit(huart, &sum, 1, 10);
    do{
        HAL_UART_Receive_DMA(huart, board->buffer, 10);
    } while(board->buffer[9] != 0xB1);
    for (int i = 0; i < 9; i++) {
        board->new_board[i / 3][i % 3] = board->buffer[i];
    }
    board->buffer[9]=0;
    board->send = (uint8_t)0xB0;
    HAL_UART_Transmit(huart, &board->send, 1, 10);
    if (Board_check(board, &board->id1, &board->id2)) {
        Board_currentMove(board, board->id1, board->id2, data);
        Board_next_turn(board);
        return;
    }
    Board_update_board(board);
}

int FirstPlay(Board *board, int *x, int *y) {
    if(board->cnt != 1) return 0;
    if(board->new_board[0][0])  {
        *x = 1, *y = 1;
    } else if(board->new_board[0][1]){
        *x = 0, *y = 0;
    } else if(board->new_board[0][2]){
        *x = 1, *y = 1;
    } else if(board->new_board[1][0]){
        *x = 0, *y = 0;
    } else if(board->new_board[1][1]){
        *x = 0, *y = 0;
    } else if(board->new_board[1][2]){
        *x = 0, *y = 2;
    } else if(board->new_board[2][0]){
        *x = 1, *y = 1;
    } else if(board->new_board[2][1]){
        *x = 0, *y = 1;
    } else if(board->new_board[2][2]){
        *x = 1, *y = 1;
    }
    return 1;
}

// 游戏主循环
int Board_play(Board *board, UART_HandleTypeDef *huart, ROBOT_DATA *data,
               KEY_DATA *Key) {
    while (board->cnt < 9 && !(board->end = evaluate(board->new_board, board->player))) {
//    	OLED_NewFrame();
//    	char t = board->turn + '0';
//    	OLED_PrintString(0, 0, &t, &font16x16, OLED_COLOR_NORMAL);
//		OLED_ShowFrame();
//		HAL_Delay(1000);
        if (board->turn == board->player) {
            OLED_NewFrame();
            OLED_PrintString(0, 0, "请您下棋", &font16x16, OLED_COLOR_NORMAL);
            OLED_ShowFrame();
            while (data->where == 0) {
                Key_scan_low(Key);
                Key_scan_high(Key);
                data->where = Key->Key_point;
            }
            data->where = 0;
//            OLED_NewFrame();
//            OLED_PrintString(0, 0, "123", &font16x16, OLED_COLOR_NORMAL);
//            OLED_ShowFrame();
//            HAL_Delay(1000);
        } else {
            OLED_NewFrame();
//            OLED_PrintString(0, 0, "机器下棋", &font16x16, OLED_COLOR_NORMAL);
            OLED_PrintString(0, 0, "机器下棋", &font16x16, OLED_COLOR_NORMAL);
            OLED_ShowFrame();
            if (board->cnt == 0) {

                Board_moveBoard(board, 5, data);
            } else if(FirstPlay(board, &board->id1, &board->id2)) {

                Board_moveBoard(board, board->id1 * 3 + board->id2 + 1, data);

            } else {
//            	t = 10;
//            	HAL_UART_Transmit(&huart1, &t, 1, 10);
//            	OLED_PrintString(0, 15, "3", &font16x16, OLED_COLOR_NORMAL);
//            	OLED_ShowFrame();
            	findBestMove(board->new_board, &board->id1, &board->id2,
                             9 - board->cnt, board->computer);
                Board_moveBoard(board, board->id1 * 3 + board->id2 + 1, data);
//                t = board->id1 * 3 + board->id2 + 1;
//                HAL_UART_Transmit(&huart1, &t, 1, 10);
            }
        }
        Board_receiveBoard(board, huart, data);
        board->cnt = count(board->new_board);
        uint8_t t = 1;
        HAL_UART_Transmit(&huart1, &t, 1, 10);
        Board_next_turn(board);
    }
    return board->end;
}
