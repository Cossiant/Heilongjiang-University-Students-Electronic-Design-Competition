#include "Usrmain.h"

// 定义一个棋子的结构体，存放数据
CHESSMAN_DATA Usr_chessman;
// 定义一个串口发送和接受数据的结构体，用于接受串口的数据
UART_DATA Usr_UART;
// 定义机器人坐标数据集
ROBOT_DATA Robot_data;

KEY_DATA Key;

// //这里即得要要注释掉
// void HAL_UART_RxCpltCallback(UART_HandleTypeDef *huart) {
//     // HAL_UART_Transmit_IT(&huart1,receiveData,2);
//     // 返回函数应该仅用作测试接口使用，当接口测试没问题应将其关闭，否则将会出现中断占用
//     HAL_UART_Receive_DMA(&huart1, (uint8_t *)&Usr_UART, UART_READ_NUM);
//     Usr_chessman.read_head = Usr_UART.head;
//     Usr_UART.head = 0xB0;  // 停止openmv发送数据
//     // 如果这里使用低延迟的执行函数，请使用阻塞式发送
//     HAL_UART_Transmit(&huart1, (uint8_t *)&Usr_UART, UART_TRAN_NUM, 1);
//     Usr_UART.ok = 1;  // 可以开始进行数据处理标志
// }

// void Robot_Move_calcu()
// {
// 角度值50~250
// Usr_chessman.calcu_data.pwm_j1 = (unsigned char)(Usr_chessman.calcu_data.j1 *
// ((150 - 48) / 90) + 48); Usr_chessman.calcu_data.pwm_j2 = (unsigned
// char)(Usr_chessman.calcu_data.j2 * ((138 - 45) / 90) + 45);
// Usr_chessman.calcu_data.pwm_j3 = (unsigned char)(Usr_chessman.calcu_data.j3 *
// ((144 - 43) / 90) + 43); Usr_chessman.calcu_data.pwm_j4 = (unsigned
// char)(Usr_chessman.calcu_data.j4 * ((146 - 50) / 90) + 50);
// Usr_chessman.calcu_data.pwm_j1 = (((Usr_chessman.calcu_data.j1 / 180) * 2 +
// 0.5) / 20 * 2000); Usr_chessman.calcu_data.pwm_j2 =
// (((Usr_chessman.calcu_data.j2 / 180) * 2 + 0.5) / 20 * 2000);
// Usr_chessman.calcu_data.pwm_j3 = (((Usr_chessman.calcu_data.j3 / 180) * 2 +
// 0.5) / 20 * 2000); Usr_chessman.calcu_data.pwm_j4 =
// (((Usr_chessman.calcu_data.j4 / 180) * 2 + 0.5) / 20 * 2000);
// Robot_Move(Usr_chessman.calcu_data.pwm_j4, 50, Mot4);
// HAL_Delay(1000);
// Robot_Move(Usr_chessman.calcu_data.pwm_j3, 43, Mot3);
// HAL_Delay(1000);
// Robot_Move(Usr_chessman.calcu_data.pwm_j2, 45, Mot2);
// HAL_Delay(1000);
// Robot_Move(Usr_chessman.calcu_data.pwm_j1, 146, Mot1);
// HAL_Delay(1000);
// }

void Usr_main()
{
    // 启动PWM定时器
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_4);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_3);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_2);
    HAL_TIM_PWM_Start(&htim4, TIM_CHANNEL_1);
    // 重置ok位为0
    Usr_UART.ok = 0;
    // 开启DMA UART接受中断
    //    这里记得注释掉
//    HAL_UART_Receive(&huart1, (uint8_t *)&Usr_UART, UART_READ_NUM);
    Robot_Move_Start(&Robot_data, &Usr_UART);

    HAL_Delay(20);
    OLED_Init();
    HAL_Delay(20);
    OLED_NewFrame();
    HAL_Delay(100);
    Key.flag = 0;
    Key.Key_point = 0;
    Key.Key_data = 0x30;
}

void task2(int cnt)
{
    // 初始化参数
    Robot_data.whitch_one = 0;
    Robot_data.where = 0;
    Key.Key_point = 0;
    // OLED初始化显示
    OLED_NewFrame();
    OLED_PrintString(0, 0, "请输入", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(0, 15, "第一个棋子颜色", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    // 读取按键直到完成
    while (Robot_data.whitch_one == 0)
    {
        Key_scan_low(&Key);
        Key_scan_high(&Key);
        Robot_data.whitch_one = Key.Key_point;
    }
    // 将相关参数重置
    Key.Key_point = 0;
    // OLED再次初始化显示
    OLED_NewFrame();
    OLED_PrintString(0, 0, "请输入", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(0, 15, "第一个棋子位置", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    // 读取按键直到完成
    while (Robot_data.where == 0)
    {
        Key_scan_low(&Key);
        Key_scan_high(&Key);
        Robot_data.where = Key.Key_point;
    }
    // 移动到棋子位置
    Robot_move_point(&Robot_data,12);
    HAL_Delay(300);
    Robot_move_point(&Robot_data, Robot_data.whitch_one <= 4 ? 10 : 11);
    HAL_Delay(300);
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
    HAL_Delay(300);
    // 回到home
    Robot_move_point(&Robot_data, 12);
    HAL_Delay(300);
    // 移动到棋盘
    Robot_move_point(&Robot_data, Robot_data.where);
    HAL_Delay(300);
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
    HAL_Delay(300);
    // 回到home
    Robot_move_point(&Robot_data, 12);
    // 第二次移动
    Robot_data.whitch_one = 0;
    Robot_data.where = 0;
    Key.Key_point = 0;
    Key.flag = 0;
    OLED_NewFrame();
    OLED_PrintString(0, 0, "请输入", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(0, 15, "第二个棋子颜色", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    while (Robot_data.whitch_one == 0)
    {
        Key_scan_low(&Key);
        Key_scan_high(&Key);
        Robot_data.whitch_one = Key.Key_point;
    }
    Key.Key_point = 0;
    OLED_NewFrame();
    OLED_PrintString(0, 0, "请输入", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(0, 15, "第二个棋子位置", &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    while (Robot_data.where == 0)
    {
        Key_scan_low(&Key);
        Key_scan_high(&Key);
        Robot_data.where = Key.Key_point;
    }
    // 移动到棋子位置
    Robot_move_point(&Robot_data,12);
    HAL_Delay(300);
    Robot_move_point(&Robot_data, Robot_data.whitch_one <= 4 ? 10 : 11);
    HAL_Delay(300);
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
    HAL_Delay(300);
    // 回到home
    Robot_move_point(&Robot_data, 12);
    HAL_Delay(300);
    // 移动到棋盘
    Robot_move_point(&Robot_data, Robot_data.where);
    HAL_Delay(300);
    HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
    HAL_Delay(300);
    // 回到home
    Robot_move_point(&Robot_data, 0);
    // 数据复位
    Key.Key_point = 0;
    Key.Key_data = 0x30;
    Key.flag = 0;
}

void Usr_whilemain()
{
    Key_scan_low(&Key);
    Key_scan_high(&Key);
    OLED_NewFrame();
    OLED_PrintString(0, 0, "主菜单", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(0, 15, "任务", &font16x16, OLED_COLOR_NORMAL);
    OLED_PrintString(32, 15, &Key.Key_data, &font16x16, OLED_COLOR_NORMAL);
    OLED_ShowFrame();
    //    任务1
    if (Key.Key_data == 0x31)
    {
        Robot_move_point(&Robot_data, 12);
        HAL_Delay(300);
        // 先移动到黑棋位置
        Robot_move_point(&Robot_data, 11);
        HAL_Delay(300);
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_SET);
        HAL_Delay(300);
        // 回到home
        Robot_move_point(&Robot_data, 12);
        HAL_Delay(300);
        // 移动到5号格
        Robot_move_point(&Robot_data, 5);
        HAL_Delay(300);
        HAL_GPIO_WritePin(Relay_GPIO_Port, Relay_Pin, GPIO_PIN_RESET);
        HAL_Delay(300);
        // 回到home
        Robot_move_point(&Robot_data, 0);
        // 数据复位
        Key.Key_point = 0;
        Key.Key_data = 0x30;
        Key.flag = 0;
    }
    else if (Key.Key_data == 0x32)
    {
        task2(1);
        task2(2);
    }
    else if (Key.Key_data == 0x34)
    {
        Robot_move_point(&Robot_data, 0);
        HAL_Delay(300);
        Robot_data.where = 0;
        Board_init(&opeBoard, 1, 2);
        uint8_t t = Board_play(&opeBoard, &huart1, &Robot_data, &Key);
        t += 16;
        HAL_UART_Transmit(&huart1, &t, 1, 10);
        Key.Key_point = 0;
        Key.Key_data = 0x30;
        Key.flag = 0;
    }
    else if (Key.Key_data == 0x35)
    {
        Robot_move_point(&Robot_data, 12);
        HAL_Delay(300);
        Robot_data.where = 0;
        Board_init(&opeBoard, 2, 1);
        Board_play(&opeBoard, &huart1, &Robot_data, &Key);
        Key.Key_point = 0;
        Key.Key_data = 0x30;
        Key.flag = 0;
    }
    //    // // 接收完成标志
    //     if (Usr_UART.ok == 1)
    //     {
    //         // 这里的delay记得删除，我目前留在程序是为了测试串口收发使用
    //         HAL_Delay(1);
    //         // 根据不同的头选择不同的对数据的处理方式
    //         switch (Usr_chessman.read_head)
    //         {
    //             // 0xAA 棋子的颜色   9
    //         case 0xFE:
    //             // Robot_data.calcu_data.X = Usr_UART.data[0];
    //             // Robot_data.calcu_data.Y = Usr_UART.data[1];
    //             // Robot_data.calcu_data.Z = Usr_UART.data[2];
    //             // // uint8_t tmp = 0x11;
    //             // // HAL_UART_Transmit(&huart1, &tmp, 1, 1);
    //             // // HAL_UART_Transmit(&huart1, (uint8_t
    //             // // HAL_UART_Transmit(&huart1, (uint8_t
    //             // // HAL_UART_Transmit(&huart1, (uint8_t
    //             // Robot_calcu(&Robot_data, &Usr_UART);
    //             // Usr_UART.data[0] = Robot_data.calcu_data.j1_data;
    //             // Usr_UART.data[1] = Robot_data.calcu_data.j2_data;
    //             // Usr_UART.data[2] = Robot_data.calcu_data.j3_data;
    //             // Usr_UART.data[3] = Robot_data.calcu_data.j4_data;
    //             // Robot_data.move_j4 = Usr_UART.data[3];
    //             // Robot_data.move_j3 = Usr_UART.data[2];
    //             // Robot_data.move_j2 = Usr_UART.data[1];
    //             // Robot_data.move_j1 = Usr_UART.data[0];
    //             // Robot_Move(&Robot_data, Mot4);
    //             // Robot_Move(&Robot_data, Mot3);
    //             // Robot_Move(&Robot_data, Mot2);
    //             // Robot_Move(&Robot_data, Mot1);
    //             // // 移动完成
    //             // HAL_Delay(1000);
    //             for (int i = 0; i < 9; i++)
    //             {
    //                 Robot_move_point(&Robot_data, i + 10);
    //                 Robot_move_point(&Robot_data, 0);
    //                 Robot_move_point(&Robot_data, i + 1);
    //                 Robot_move_point(&Robot_data, 0);
    //             }
    //             Usr_UART.head = 0xFE;
    //             HAL_UART_Transmit(&huart1, (uint8_t *)&Usr_UART, UART_READ_NUM,
    //             1);
    //
    //             break;
    //             // 这是测试选项，用于输出结果判断是否处理完成
    //         case 0xFF:
    //             // 中心点坐标 145
    //             // 起始点坐标 48
    //             // 终值点坐标 238
    //             // 电机1
    //             // 中心点坐标 138
    //             // 起始点坐标 45
    //             // 终值点坐标 232
    //             // 电机2
    //             // 中心点坐标 138
    //             // 起始点坐标 43
    //             // 终值点坐标 200
    //             // 电机3
    //             // 中心点坐标 150
    //             // 起始点坐标 50
    //             // 终值点坐标 238
    //             // 电机4
    //             // 移动电机
    //             // __HAL_TIM_SetCompare(&htim4, Mot2, Usr_UART.data[0]); //
    ////             首先进行电机2的初始化
    //
    //             // 首先读取电机要移动到的目标数据
    //             Robot_data.move_j4 = Usr_UART.data[0];
    //             Robot_data.move_j3 = Usr_UART.data[1];
    //             Robot_data.move_j2 = Usr_UART.data[2];
    //             Robot_data.move_j1 = Usr_UART.data[3];
    //             Robot_data.mod = Usr_UART.data[4];
    //             // 执行移动函数
    //             // 0号模式，移动到指定位置模式
    //             if (Robot_data.mod == 0x00)
    //             {
    //                 Robot_Move(&Robot_data, Mot1);
    //                 Robot_Move(&Robot_data, Mot4);
    //                 Robot_Move(&Robot_data, Mot3);
    //                 Robot_Move(&Robot_data, Mot2);
    //             }
    //             // 1号模式，从棋盘到其他地方使用
    //             else if (Robot_data.mod == 0x01)
    //             {
    //                 Robot_Move(&Robot_data, Mot2);
    //                 Robot_Move(&Robot_data, Mot3);
    //                 Robot_Move(&Robot_data, Mot4);
    //                 Robot_Move(&Robot_data, Mot1);
    //             }
    //             // 2号模式，初始化
    //             else if (Robot_data.mod == 0x02)
    //             {
    //                 Robot_Move(&Robot_data, Mot1);
    //                 Robot_Move(&Robot_data, Mot2);
    //                 Robot_Move(&Robot_data, Mot3);
    //                 Robot_Move(&Robot_data, Mot4);
    //             }
    //             // 移动完成
    //             HAL_Delay(1000);
    //             Usr_UART.head = 0xFF;
    //             HAL_UART_Transmit(&huart1, (uint8_t *)&Usr_UART, UART_TRAN_NUM,
    //             1); break;
    //         default:
    //             break;
    //         }
    //         Usr_UART.ok = 0; // 完成数据的处理
    //     }

    // if ((Usr_UART.head == 0xA1) && (Usr_UART.ok == 1))
    // {
    //     for (int i = 0; i < 9; i++)
    //     {
    //         //这里是控制PWM波形进行变换的函数
    //         __HAL_TIM_SetCompare(&htim4, TIM_CHANNEL_4, Usr_UART.data[i] +
    //         5); HAL_Delay(1000);
    //     }
    //     Usr_UART.head = 0xB2;
    //     // 数据处理完成，返回标志位并返回处理数据结果
    //     Usr_UART.ok = 0;
    //     // 如果这里使用低延迟的执行函数，请使用阻塞式发送
    //     HAL_UART_Transmit(&huart1, (uint8_t *)&Usr_UART, UART_DATA_NUM);
    // }
}
