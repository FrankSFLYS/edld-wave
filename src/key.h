/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : key.h
http://frank.xin
**********************************************************************/
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

int mKeyScan();

#endif