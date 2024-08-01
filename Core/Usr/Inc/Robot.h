/*
 * Robot.h
 *
 *  Created on: Jul 31, 2024
 *      Author: Cossiant
 */

#ifndef USR_INC_ROBOT_H_
#define USR_INC_ROBOT_H_

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <math.h>
#include "data.h"
#include "get_angle.h"

// 实现机器人根据输入的角度参数进行移动
void Robot_Move(ROBOT_DATA *data, unsigned int Mot);
// 实现机器人移动的初始化
void Robot_Move_Start(ROBOT_DATA *data,UART_DATA *Usr_UART);
// 未成功实现机器人的运动方程逆解
void Robot_calcu(ROBOT_DATA *data, UART_DATA *Usr_UART);
// 实现机器人点对点移动
void Robot_move_point(ROBOT_DATA *data, unsigned char point);

void Robot_Up(ROBOT_DATA *data);

void Robot_Down(ROBOT_DATA *data);
#endif /* USR_INC_ROBOT_H_ */
