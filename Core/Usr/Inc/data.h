/*
 * data.h
 *
 *  Created on: Jul 31, 2024
 *      Author: Cossiant
 */

#ifndef USR_INC_DATA_H_
#define USR_INC_DATA_H_

#define Mot1 TIM_CHANNEL_4
#define Mot2 TIM_CHANNEL_3
#define Mot3 TIM_CHANNEL_2
#define Mot4 TIM_CHANNEL_1

#define UART_READ_NUM 10
#define UART_TRAN_NUM 1
#include <stdint.h>

typedef struct
{
    uint8_t X, Y, Z;
    double j1, j2, j3, j4;
    unsigned char j1_data,j2_data,j3_data,j4_data;
} CALCU_DATA;

typedef struct
{
    // 移动到的目标的坐标
    unsigned char move_j1,move_j2,move_j3,move_j4;
    // 当前机器人坐标
    unsigned char now_j1,now_j2,now_j3,now_j4;
    
    CALCU_DATA calcu_data;
    unsigned char whitch_one;
    unsigned char where;
    unsigned char mod;
}ROBOT_DATA;

typedef struct
{
    // 读取到的操作头
    char read_head;
    // 棋子的颜色数组
    char color[9];
} CHESSMAN_DATA;

typedef struct
{
    char head; // 硬件接受头
    // 作为接收方，需要收到的头
    // 0xAA 棋子的颜色   9
    // 0xAB 棋子的位置(x)9
    // 0xAC 棋子的位置(y)9
    // 0xAD
    // 0xAE
    // 0xAF
    // 作为发送方，需要发送的头
    // 0xB0 stm32接收完成
    // 0xB1 stm32准备继续接收AA
    // 0xB2 stm32准备继续接收AB
    // 0xB3 stm32准备继续接收AC
    // 0xB4 stm32全部处理完毕
    // 0xB5
    // 0xB6
    char data[9]; // 数据接收
    char ok;      // 本地串口数据处理完成标志位
} UART_DATA;

typedef struct
{
//	flag为1时表示被占用
//	flag为0时表示就绪
	unsigned char flag;
//	keypoint记录的是当前读取到的按键的编号
	char Key_point;
	char Key_data;
}KEY_DATA;

#endif /* USR_INC_DATA_H_ */
