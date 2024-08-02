/*
 * Robot.c
 *
 *  Created on: Jul 31, 2024
 *      Author: Cossiant
 */

#include "Robot.h"
#include "chess_stack.h"

#ifndef BLACK

#define BLACK 1
#define WHITE 2

#endif

// 机器人移动
void Robot_Move(ROBOT_DATA *data, unsigned int Mot)
{
    unsigned char move_j, now_j;
    // 根据选择的电机进行不同的数据读取和处理
    switch (Mot)
    {
    case Mot1:
        // 读取目标值的数据
        move_j = data->move_j1;
        // 读取当前坐标数据
        now_j = data->now_j1;
        // 将目标值的数据传递给当前坐标数据以方便下次使用
        data->now_j1 = data->move_j1;
        break;
    case Mot2:
        move_j = data->move_j2;
        now_j = data->now_j2;
        data->now_j2 = data->move_j2;
        break;
    case Mot3:
        move_j = data->move_j3;
        now_j = data->now_j3;
        data->now_j3 = data->move_j3;
        break;
    case Mot4:
        move_j = data->move_j4;
        now_j = data->now_j4;
        data->now_j4 = data->move_j4;
        break;
    default:
        break;
    }
    // 判断当前的目标值和当前值来进行不同的移动
    if (move_j >= now_j)
    {
        for (unsigned char i = 0; i < move_j - now_j; i++)
        {
            __HAL_TIM_SetCompare(&htim4, Mot, now_j + i); //
            HAL_Delay(15);
        }
    }
    else if (move_j < now_j)
    {
        for (unsigned char i = 0; i < now_j - move_j; i++)
        {
            __HAL_TIM_SetCompare(&htim4, Mot, now_j - i); //
            HAL_Delay(15);
        }
    }
}

void Robot_Up(ROBOT_DATA *data)
{
}

void Robot_Down(ROBOT_DATA *data)
{
}

// 机器人初始化移动
void Robot_Move_Start(ROBOT_DATA *data, UART_DATA *Usr_UART)
{
    // 初始化顺序，先进行电机2初始化，然后电机3，其次电机4，最后电机1
    __HAL_TIM_SetCompare(&htim4, Mot1, 220); // 电机1的初始化
    HAL_Delay(1000);
    __HAL_TIM_SetCompare(&htim4, Mot2, 45); // 首先进行电机2的初始化
    HAL_Delay(1000);
    __HAL_TIM_SetCompare(&htim4, Mot3, 45); // 电机3的初始化
    HAL_Delay(1000);
    __HAL_TIM_SetCompare(&htim4, Mot4, 45); // 电机4的初始化
    HAL_Delay(1000);
    // 位置初始化
    data->now_j1 = 220;
    data->now_j2 = 45;
    data->now_j3 = 45;
    data->now_j4 = 45;
    // FF D0 80 50 8D 01 FF FF FF FF
    data->move_j4 = 0xD0;
    data->move_j3 = 0x80;
    data->move_j2 = 0x50;
    data->move_j1 = 0xDC;
    data->mod = 0x01;
    // E0 64 64 8D
    // 前往home位置，完成初始化
    Robot_Move(data, Mot2);
    Robot_Move(data, Mot3);
    Robot_Move(data, Mot4);
    Robot_Move(data, Mot1);
    // 完成初始化
//    Usr_UART->head = 0x00; // 重新循环
//    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)Usr_UART, UART_TRAN_NUM);

    Stack_init(&whiteStack);
    Stack_init(&blackStack);
    // FF D0 80 50 8D 01 FF FF FF FF
    // FF AC 66 84 A6 00 FF FF FF FF
    // FF AC 7A 7A A8 00 FF FF FF FF
    // FF 9A 8A 78 AD 00 FF FF FF FF
    // FF 90 9A 76 B0 00 FF FF FF FF
    // FF 7D A6 78 B3 00 FF FF FF FF
    
    Stack_push(&whiteStack, 0x7D, 0xA6, 0x78, 0xB3);
    Stack_push(&whiteStack, 0x90, 0x9A, 0x76, 0xB0);
    Stack_push(&whiteStack, 0x9A, 0x8A, 0x78, 0xAD);
    Stack_push(&whiteStack, 0xAC, 0x7A, 0x7A, 0xA8);
    Stack_push(&whiteStack, 0xAC, 0x66, 0x84, 0xA6);

    // FF D0 80 50 8D 01 FF FF FF FF
    // FF AC 66 84 74 00 FF FF FF FF
    // FF AC 7A 7A 72 00 FF FF FF FF
    // FF 9A 8A 78 6D 00 FF FF FF FF
    // FF 90 9A 76 68 00 FF FF FF FF
    // FF 7D A6 78 60 00 FF FF FF FF

    Stack_push(&blackStack, 0x7D, 0xA6, 0x78, 0x60);
    Stack_push(&blackStack, 0x90, 0x9A, 0x76, 0x68);
    Stack_push(&blackStack, 0x9A, 0x8A, 0x78, 0x6D);
    Stack_push(&blackStack, 0xAC, 0x7A, 0x7A, 0x72);
    Stack_push(&blackStack, 0xAC, 0x66, 0x84, 0x74);
}

// 机器人坐标位置计算
void Robot_calcu(ROBOT_DATA *data, UART_DATA *Usr_UART)
{
    get_angle(data->calcu_data.X, data->calcu_data.Y, data->calcu_data.Z, &data->calcu_data.j1, &data->calcu_data.j2, &data->calcu_data.j3, &data->calcu_data.j4, Usr_UART);
    // Usr_UART->head = 0xFE;
    // HAL_UART_Transmit(&huart1, (uint8_t *)Usr_UART, UART_TRAN_NUM, 1);
    data->calcu_data.j1_data = (((data->calcu_data.j1 / 180) * 2 + 0.5) / 20 * 2000);
    data->calcu_data.j2_data = (((data->calcu_data.j2 / 180) * 2 + 0.5) / 20 * 2000);
    data->calcu_data.j3_data = (((data->calcu_data.j3 / 180) * 2 + 0.5) / 20 * 2000);
    data->calcu_data.j4_data = (((data->calcu_data.j4 / 180) * 2 + 0.5) / 20 * 2000);
}

// 机器人点移动
void Robot_move_point(ROBOT_DATA *data, unsigned char point)
{ // 初始化位置
    switch (point)
    {
    case 1:
        // FF AC 79 7B 9A 00 FF FF FF FF
        //
        data->move_j4 = 0xAC;
        data->move_j3 = 0x79;
        data->move_j2 = 0x7B;
        data->move_j1 = 0x9A;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 2:
        // FF B0 7A 7A 8E 00 FF FF FF FF
        data->move_j4 = 0xB0;
        data->move_j3 = 0x7A;
        data->move_j2 = 0x7A;
        data->move_j1 = 0x8E;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 3:
        // FF B0 78 7B 80 00 FF FF FF FF
        data->move_j4 = 0xB0;
        data->move_j3 = 0x78;
        data->move_j2 = 0x7B;
        data->move_j1 = 0x80;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 4:
        // FF 90 96 76 9C 00 FF FF FF FF
        data->move_j4 = 0x90;
        data->move_j3 = 0x96;
        data->move_j2 = 0x76;
        data->move_j1 = 0x9C;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 5:
        // FF 90 96 76 8E 00 FF FF FF FF
        data->move_j4 = 0x90;
        data->move_j3 = 0x96;
        data->move_j2 = 0x76;
        data->move_j1 = 0x8E;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 6:
        // FF 90 95 76 7E 00 FF FF FF FF
        data->move_j4 = 0x90;
        data->move_j3 = 0x95;
        data->move_j2 = 0x76;
        data->move_j1 = 0x7E;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 7:
        // FF 80 A8 76 9E 00 FF FF FF FF
        data->move_j4 = 0x80;
        data->move_j3 = 0xA8;
        data->move_j2 = 0x76;
        data->move_j1 = 0x9E;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 8:
        // FF 80 AA 76 8A 00 FF FF FF FF
        data->move_j4 = 0x80;
        data->move_j3 = 0xAA;
        data->move_j2 = 0x76;
        data->move_j1 = 0x8A;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 9:
        // FF 80 A8 76 78 00 FF FF FF FF
        data->move_j4 = 0x80;
        data->move_j3 = 0xA8;
        data->move_j2 = 0x76;
        data->move_j1 = 0x78;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 10:
        Stack_pop(&whiteStack, data);
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 11:
        Stack_pop(&blackStack, data);
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 12:
        // FF D0 80 50 8D 01 FF FF FF FF
        data->move_j4 = 0xD0;
        data->move_j3 = 0x80;
        data->move_j2 = 0x50;
        data->move_j1 = 0x8D;
        data->mod = 0x01;
        Robot_Move(data, Mot2);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot1);
        break;
    // case 12:
    //     data->move_j4 = 0xA0;
    //     data->move_j3 = 0x8A;
    //     data->move_j2 = 0x76;
    //     data->move_j1 = 0xAA;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 13:
    //     data->move_j4 = 0x90;
    //     data->move_j3 = 0x9A;
    //     data->move_j2 = 0x75;
    //     data->move_j1 = 0xAF;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 14:
    //     data->move_j4 = 0x80;
    //     data->move_j3 = 0xA4;
    //     data->move_j2 = 0x76;
    //     data->move_j1 = 0xB2;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 15:
    //     data->move_j4 = 0xAD;
    //     data->move_j3 = 0x66;
    //     data->move_j2 = 0x85;
    //     data->move_j1 = 0x72;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 16:
    //     data->move_j4 = 0xAD;
    //     data->move_j3 = 0x7B;
    //     data->move_j2 = 0x7B;
    //     data->move_j1 = 0x6D;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 17:
    //     data->move_j4 = 0xA0;
    //     data->move_j3 = 0x8B;
    //     data->move_j2 = 0x76;
    //     data->move_j1 = 0x6A;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 18:
    //     data->move_j4 = 0x90;
    //     data->move_j3 = 0x9A;
    //     data->move_j2 = 0x75;
    //     data->move_j1 = 0x64;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    // case 19:
    //     data->move_j4 = 0x80;
    //     data->move_j3 = 0xA4;
    //     data->move_j2 = 0x76;
    //     data->move_j1 = 0x5A;
    //     data->mod = 0x00;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot4);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot2);
    //     break;
    case 0:
        // FF D0 80 50 8D 01 FF FF FF FFS
        data->move_j4 = 0xD0;
        data->move_j3 = 0x80;
        data->move_j2 = 0x50;
        data->move_j1 = 0xDC;
        data->mod = 0x01;
        Robot_Move(data, Mot2);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot1);
        break;
    // case 99:
    //     data->move_j4 = 0x90;
    //     data->move_j3 = 0x2B;
    //     data->move_j2 = 0x2E;
    //     data->move_j1 = 0xDC;
    //     data->mod = 0x02;
    //     Robot_Move(data, Mot1);
    //     Robot_Move(data, Mot2);
    //     Robot_Move(data, Mot3);
    //     Robot_Move(data, Mot4);
    //     break;
    default:
        break;
    }
}
