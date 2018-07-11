/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : main.c
File target: STC89C52 with PCF8591 ADDA Converter
Approach  : To generate a series of waves including sin(SIN),
triangle(TRI), sawtooth(SAW), square(SQR) whose amplitude and their
frequency can be adjusted by pressing keys. Details can be found at
http://frank.xin/edld
**********************************************************************/
#include <reg52.h>
#include <intrins.h>
#include "timer.h"
#include "key.h"
#include "pcf8591.h"

#define MODE_SIN KEY_SIN
#define MODE_TRI KEY_TRI
#define MODE_SAW KEY_SAW
#define MODE_SQR KEY_SQR
#define WAVE_SIZE 32
#define MAX_VOLT ((float)5.0)
#define DEFAULT_TL (65536-251)%256
#define DEFAULT_TH (65536-251)/256
#define FREQUENCY_STEP_R 28
#define FREQUENCY_STEP_A 7

// Generate sin waveForm
void genSin();
// Generate Triangle waveForm
void genTri();
// Generate Sawtooth waveForm
void genSaw();
// Generate Square waveForm
void genSqr();

// index of indexing waveForm
unsigned char index;

unsigned char i;
unsigned char keyCode;
unsigned int initValue;
unsigned int tlValue, thValue;
float voltage;

void (*generater)(void);

// Table of sin wave
unsigned char sin[] = {
    //128,176,218,245,255,245,218,176,128,79,37,10,0,10,37,79
    128, 152, 176, 198, 218, 234, 245, 253,
    255, 253, 245, 234, 218, 198, 176, 152,
    128, 103,  79,  57,  37,  21,  10,   2,  
      0,   2,  10,  21,  37,  57,  79, 103
};

volatile unsigned char waveForm[WAVE_SIZE];

void init() {
    Init_Timer0();
    DACStart();
    P1 = 0xfe;
    voltage = 5.0;
    initValue = 251;
    tlValue = (65536-initValue)%256;
    thValue = (65536-initValue)/256;
    generater = genSin;
    generater();
}
void main() {
    init();
    while(1) {
        keyCode = mKeyScan();
        switch(keyCode) {
            case KEY_NULL:
                break;
            case KEY_ONOFF:
                break;
            case KEY_SIN:
                generater = genSin;
                break;
            case KEY_SQR:
                generater = genSqr;
                break;
            case KEY_SAW:
                generater = genSaw;
                break;
            case KEY_TRI:
                generater = genTri;
                break;
            case KEY_FRE_R_ADD:
                initValue -= FREQUENCY_STEP_R;
                break;
            case KEY_FRE_R_SUB:
                initValue += FREQUENCY_STEP_R;
                break;
            case KEY_FRE_A_ADD:
                initValue -= FREQUENCY_STEP_A;
                break;
            case KEY_FRE_A_SUB:
                initValue += FREQUENCY_STEP_A;
                break;
            case KEY_AMP_R_ADD:
                voltage += 0.1;
                break;
            case KEY_AMP_R_SUB:
                voltage -= 0.1;
                break;
            case KEY_AMP_A_ADD:
                voltage += 0.01;
                break;
            case KEY_AMP_A_SUB:
                voltage -= 0.01;
                break;
            default:
                break;
        }
        if(voltage < 0) {
            voltage = 0.1;
        }
        if(voltage > 5.0) {
            voltage = 5.0;
        }
        if(initValue < 251) {
            initValue = 251;
        }
        if(initValue > 65534) {
            initValue = 65534;
        }
        tlValue = (65536-initValue)%256;
        thValue = (65536-initValue)/256;
        generater();
    }
}

void onTimer0() interrupt 1 {
	TL0 = tlValue;   // 200 Hz for sin wave
	TH0 = thValue;   // 200 Hz for sin wave
    index ++;
    if(index == WAVE_SIZE) {
        index = 0;
    }
    ContinuesSend(waveForm[index]);
}

void genSaw() {
    for(i = 0; i < WAVE_SIZE; i++) {
        waveForm[i] = i*(255/WAVE_SIZE)*(voltage/MAX_VOLT);
    }
}

void genSin() {
    for(i = 0; i < WAVE_SIZE; i++) {
        waveForm[i] = sin[i]*(voltage/MAX_VOLT);
    }
}

void genSqr() {
    for(i = 0; i < WAVE_SIZE/2; i++) {
        waveForm[i] = (255)*(voltage/MAX_VOLT);
    }
    for(i = WAVE_SIZE/2; i < WAVE_SIZE; i++) {
        waveForm[i] = 0;
    }
}

void genTri() {
    for(i = 0; i < WAVE_SIZE/2; i++) {
        waveForm[i] = (WAVE_SIZE/2 + i*(255/WAVE_SIZE*2))*(voltage/MAX_VOLT);
    }
    for(i = WAVE_SIZE/2; i < WAVE_SIZE; i++) {
        waveForm[i] = 255*(voltage/MAX_VOLT) - waveForm[i - WAVE_SIZE/2];
    }
}