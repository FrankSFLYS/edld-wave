#ifndef _KEY_H_

#define _KEY_H_
#define KEY_ONOFF    16
#define KEY_NULL      0
#define KEY_FRE_R_ADD 1
#define KEY_FRE_R_SUB 5
#define KEY_FRE_A_ADD 2
#define KEY_FRE_A_SUB 6
#define KEY_AMP_R_ADD 3
#define KEY_AMP_R_SUB 7
#define KEY_AMP_A_ADD 4
#define KEY_AMP_A_SUB 8
#define KEY_SIN       9
#define KEY_SQR      10
#define KEY_TRI      11
#define KEY_SAW      12

#include <reg52.h>

sbit _KEY_H_K1  = P3^4;

sbit _KEY_H_kr0 = P3^0;
sbit _KEY_H_kr1 = P3^1;
sbit _KEY_H_kr2 = P3^2;
sbit _KEY_H_kr3 = P3^3;

sbit _KEY_H_kc0 = P3^4;
sbit _KEY_H_kc1 = P3^5;
sbit _KEY_H_kc2 = P3^6;
sbit _KEY_H_kc3 = P3^7;

//void keyInit(int *);
//int keyScan();
int mKeyScan();
//void EnterSettingsMode() interrupt 0;

#endif