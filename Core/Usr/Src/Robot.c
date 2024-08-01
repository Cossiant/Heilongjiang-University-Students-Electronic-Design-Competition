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

void Robot_Up(ROBOT_DATA *data){

}

void Robot_Down(ROBOT_DATA *data){

}

// 机器人初始化移动
void Robot_Move_Start(ROBOT_DATA *data, UART_DATA *Usr_UART)
{
    // 初始化顺序，先进行电机2初始化，然后电机3，其次电机4，最后电机1
    __HAL_TIM_SetCompare(&htim4, Mot1, 220); // 电机1的初始化
    HAL_Delay(1000);
    __HAL_TIM_SetCompare(&htim4, Mot2, 100); // 首先进行电机2的初始化
    HAL_Delay(1000);
    __HAL_TIM_SetCompare(&htim4, Mot3, 100); // 电机3的初始化
    HAL_Delay(1000);
    __HAL_TIM_SetCompare(&htim4, Mot4, 220); // 电机4的初始化
    HAL_Delay(1000);
    // 位置初始化
    data->now_j1 = 220;
    data->now_j2 = 100;
    data->now_j3 = 100;
    data->now_j4 = 220;
    data->move_j4 = 0xE0;
    data->move_j3 = 0x64;
    data->move_j2 = 0x64;
    data->move_j1 = 0xDC;
    data->mod = 0x01;
    // E0 64 64 8D
    // 前往home位置，完成初始化
    Robot_Move(data, Mot2);
    Robot_Move(data, Mot3);
    Robot_Move(data, Mot4);
    Robot_Move(data, Mot1);
    // 完成初始化
    Usr_UART->head = 0x00; // 重新循环
    HAL_UART_Transmit_DMA(&huart1, (uint8_t *)Usr_UART, UART_TRAN_NUM);
    
    Stack_init(&whiteStack);
    Stack_init(&blackStack);
    Stack_push(&whiteStack, 0x7D, 0xA5, 0x85, 0xC9);
    Stack_push(&whiteStack, 0x93, 0x98, 0x82, 0xC0);
    Stack_push(&whiteStack, 0x9B, 0x8B, 0x86, 0xBA);
    Stack_push(&whiteStack, 0x9B, 0x79, 0x8E, 0xB4);
    Stack_push(&whiteStack, 0xAA, 0x5E, 0x95, 0xB0);


    Stack_push(&blackStack, 0x7D, 0xAD, 0x81, 0x60);
    Stack_push(&blackStack, 0x8E, 0xA0, 0x80, 0x69);
    Stack_push(&blackStack, 0xA0, 0x8F, 0x80, 0x70);
    Stack_push(&blackStack, 0xAD, 0x7B, 0x8A, 0x78);
    Stack_push(&blackStack, 0xAd, 0x62, 0x92, 0x7C);

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
    // FF 90 2B 2E DC 02 FF FF FF FF
    // home等待位置
    // FF E0 64 64 8C 01 FF FF FF FF
    // 4号点
    // FF 90 94 78 9A 00 FF FF FF FF
    // 5号点
    // FF 90 94 78 88 00 FF FF FF FF
    // 6号点
    // FF 90 94 78 7A 00 FF FF FF FF
    // 2号点
    // FF B0 7D 7A 8A 00 FF FF FF FF
    // 3号点
    // FF AE 7B 7B 7F 00 FF FF FF FF
    // 1号点
    // FF B0 7B 7B 99 00 FF FF FF FF
    // 8号点
    // FF 80 AA 76 86 00 FF FF FF FF
    // 9号点
    // FF 80 A8 78 72 00 FF FF FF FF
    // 7号点
    // FF 80 A8 76 9B 00 FF FF FF FF
    // 物品1点
    // FF AD 66 84 A6 00 FF FF FF FF
    // 物品2点
    // FF AD 7B 7B A7 00 FF FF FF FF
    // 物品3点
    // FF A0 8A 76 AA 00 FF FF FF FF
    // 物品4点
    // FF 90 9A 75 AF 00 FF FF FF FF
    // 物品5点
    // FF 80 A4 76 B2 00 FF FF FF FF
    // 物品6点
    // FF AD 66 85 72 00 FF FF FF FF
    // 物品7点
    // FF AD 7B 7B 6D 00 FF FF FF FF
    // 物品8点
    // FF A0 8B 76 6A 00 FF FF FF FF
    // 物品9点
    // FF 90 9A 75 64 00 FF FF FF FF
    // 物品10点
    // FF 80 A4 76 5A 00 FF FF FF FF
    switch (point)
    {
    case 1:
        data->move_j4 = 0x92;
        data->move_j3 = 0x92;
        data->move_j2 = 0x80;
        data->move_j1 = 0xA0;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 2:
        data->move_j4 = 0x8E;
        data->move_j3 = 0x97;
        data->move_j2 = 0x82;
        data->move_j1 = 0x94;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 3:
        data->move_j4 = 0x92;
        data->move_j3 = 0x96;
        data->move_j2 = 0x7D;
        data->move_j1 = 0x84;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 4:
        data->move_j4 = 0x82;
        data->move_j3 = 0xA8;
        data->move_j2 = 0x7D;
        data->move_j1 = 0xA5;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 5:
        data->move_j4 = 0x82;
        data->move_j3 = 0xA8;
        data->move_j2 = 0x80;
        data->move_j1 = 0x94;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 6:
        data->move_j4 = 0x82;
        data->move_j3 = 0xAB;
        data->move_j2 = 0x7B;
        data->move_j1 = 0x7C;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 7:
        data->move_j4 = 0x6E;
        data->move_j3 = 0xBD;
        data->move_j2 = 0x80;
        data->move_j1 = 0xA8;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 8:
        data->move_j4 = 0x6E;
        data->move_j3 = 0xC2;
        data->move_j2 = 0x80;
        data->move_j1 = 0x8E;
        data->mod = 0x00;
        Robot_Move(data, Mot1);
        Robot_Move(data, Mot4);
        Robot_Move(data, Mot3);
        Robot_Move(data, Mot2);
        break;
    case 9:
        data->move_j4 = 0x6E;
        data->move_j3 = 0xBE;
        data->move_j2 = 0x7E;
        data->move_j1 = 0x76;
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
        data->move_j4 = 0xE0;
        data->move_j3 = 0x64;
        data->move_j2 = 0x64;
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
        data->move_j4 = 0xE0;
        data->move_j3 = 0x64;
        data->move_j2 = 0x64;
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
