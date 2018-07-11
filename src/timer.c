/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : timer.c
File target: STC89C52 with PCF8591 ADDA Converter
Approach  : Initializing timer0 and start working
http://frank.xin
**********************************************************************/
#include "timer.h"
#include <reg52.h>

void Init_Timer0(void) {
    TMOD = 0x01;		  // Set timer mod as 16-bit with non-reloading
	TL0 = (65536-251)%256; // Period of first timing
	TH0 = (65536-251)/256; // Period of first timing
	TF0 = 0;        // Clear flow remark
    ET0 = 1;        // Enable timer0 interrupt
    EA  = 1;        // Enable global interrupt
	TR0 = 1;		// Start timing
}