
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include "data.h"
#include "get_angle.h"
#include "usart.h"
#define PI 3.1415927
#define tr(a) ((a) * PI / 180)
#define ta(a) ((a) * 180 / PI)
#define CR 57.2957795

void get_angle(uint8_t X, uint8_t Y, uint8_t Z, double *ansj1, double *ansj2, double *ansj3, double *ansj4, UART_DATA *Usr_UART)
{
    double P, a1, a2, a3, a4;
    P = 4.5,
    a1 = 10,
    a2 = 11,
    a3 = 10,
    a4 = 4;
    double mn = 1e9;
    *ansj1 = X == 0 ? 90 : ta(atan((Y + P) / X));

    for (double j3 = 0; j3 <= 180; j3 += 0.1) {
        double K1 = a2 + a3 * cos(tr(j3));
        double K2 = a3 * sin(tr(j3));

        double H = Z - a1, L = sqrt((Y + P) * (Y + P) + X * X) - a4;
        double Cos2 = (K1 * H + K2 * L) / (K1 * K1 + K2 * K2);
        double Sin2 = sqrt(1 - Cos2 * Cos2);

        double j2 = ta(atan(Sin2 / Cos2));
        double j4 = 270 - j2 - j3;

        if (j2 < 0 || j2 > 180 || j4 < 0 || j4 > 180) continue;
        double tmp1 = a1 + a2 * cos(tr(j2)) + a3 * cos(tr(j2 + j3)) - Z,
               tmp2 = a2 * sin(tr(j2)) + a3 * sin(tr(j2 + j3)) + a4 -
                      sqrt((Y + P) * (Y + P) + X * X);
        // printf("j2: %.2lf\nj3: %.2lf\nj4: %.2lf\n", j2, j3, j4);

        // printf("tmp1: %.2lf\ntmp2: %.2lf\n", tmp1, tmp2);
        // if (fabsl(tmp1 - Z) < EPS && fabsl(tmp2) < EPS) {
            if (tmp1 * tmp1 + tmp2 * tmp2 < mn) {
                mn = tmp1 * tmp1 + tmp2 * tmp2;
                *ansj2 = j2;
                *ansj3 = j3;
                *ansj4 = j4;
            }
        // }
    }
}
