/*
 * key.c
 *
 *  Created on: Aug 1, 2024
 *      Author: Cossiant
 */

#include "key.h"

void Key_scan_low(KEY_DATA *data)
{
	if (data->flag == 1)
		return;
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 0;
			data->Key_data = 0 + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 1;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 2;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 3;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 4;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 5;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 6;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 7;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY9_GPIO_Port, KEY9_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY9_GPIO_Port, KEY9_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 8;
			data->Key_data = data->Key_point + 0x30;
		}
	}
	if (HAL_GPIO_ReadPin(KEY10_GPIO_Port, KEY10_Pin) == 0)
	{
		HAL_Delay(10);
		if (HAL_GPIO_ReadPin(KEY10_GPIO_Port, KEY10_Pin) == 0)
		{
			data->flag = 1;
			data->Key_point = 9;
			data->Key_data = data->Key_point + 0x30;
		}
	}
}

void Key_scan_high(KEY_DATA *data)
{
	if (data->flag == 0)
		return;
	if (HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin) == 1)
	{
		if (HAL_GPIO_ReadPin(KEY2_GPIO_Port, KEY2_Pin) == 1)
		{
			if (HAL_GPIO_ReadPin(KEY3_GPIO_Port, KEY3_Pin) == 1)
			{
				if (HAL_GPIO_ReadPin(KEY4_GPIO_Port, KEY4_Pin) == 1)
				{
					if (HAL_GPIO_ReadPin(KEY5_GPIO_Port, KEY5_Pin) == 1)
					{
						if (HAL_GPIO_ReadPin(KEY6_GPIO_Port, KEY6_Pin) == 1)
						{
							if (HAL_GPIO_ReadPin(KEY7_GPIO_Port, KEY7_Pin) == 1)
							{
								if (HAL_GPIO_ReadPin(KEY8_GPIO_Port, KEY8_Pin) == 1)
								{
									if (HAL_GPIO_ReadPin(KEY9_GPIO_Port, KEY9_Pin) == 1)
									{
										if (HAL_GPIO_ReadPin(KEY10_GPIO_Port, KEY10_Pin) == 1)
										{
											data->flag = 0;
											data->Key_point = 0;
											data->Key_data = 0x30;
										}
									}
								}
							}
						}
					}
				}
			}
		}
	}
}
