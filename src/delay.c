/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : delay.c
File target: STC89C52 with PCF8591 ADDA Converter
Approach  : Delay by none-instructions
http://frank.xin
**********************************************************************/
#include "delay.h"

unsigned char __j_, __k_;

void delay(unsigned char i)	{
	for(__j_ = i; __j_ > 0; __j_--)
		for(__k_ = 32; __k_ > 0; __k_--);
}
