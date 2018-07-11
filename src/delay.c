#include "delay.h"
#include <intrins.h>

unsigned char __j_, __k_;

void delay(unsigned char i)	{
	for(__j_ = i; __j_ > 0; __j_--)
		for(__k_ = 32; __k_ > 0; __k_--);
}

// void delay100us()		//@11.0592MHz
// {
// 	_nop_();
// 	_nop_();
// 	__j_ = 2;
// 	__k_ = 15;
// 	do
// 	{
// 		while (--__k_);
// 	} while (--__j_);
// }

// void delay1ms()		//@11.0592MHz
// {
// 	unsigned char __j_, __k_;

// 	_nop_();
// 	_nop_();
// 	_nop_();
// 	__j_ = 11;
// 	__k_ = 190;
// 	do
// 	{
// 		while (--__k_);
// 	} while (--__j_);
// }

// void delay3906us()		//@11.0592MHz
// {
// 	unsigned char __j_, __k_;

// 	_nop_();
// 	__j_ = 43;
// 	__k_ = 1;
// 	do
// 	{
// 		while (--__k_);
// 	} while (--__j_);
// }
