#ifndef _GET_ANGLE_H
#define _GET_ANGLE_H

#include "main.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include <string.h>
#include <math.h>
#include "data.h"
#include "usart.h"

void get_angle(uint8_t X, uint8_t Y, uint8_t Z, double *ansj1, double *ansj2, double *ansj3, double *ansj4, UART_DATA *Usr_UART);
// int main() {
// 	double X, Y, Z, j1, j2, j3, j4;
// 	scanf("%lf %lf %lf", &X, &Y, &Z);
// 	if(!get_angle(X, Y, Z, &j1, &j2, &j3, &j4)){
// 		printf("j1:%f,j2:%f\nj3:%f,j4:%f\n", j1, j2, j3, j4);
// 	}
// 	else{
// 		printf("No solution\n");
// 	}
// }

#endif
