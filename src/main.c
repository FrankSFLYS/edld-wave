/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : main.c
File target: STC89C52 with PCF8591 ADDA Converter
Approach  : To generate a series of waves including sin(SIN),
triangle(TRI), sawtooth(SAW), square(SQR) whose amplitude and their
frequency can be adjusted by pressing keys. Details can be found at
http://frank.xin
**********************************************************************/
#include <reg52.h>
#include <intrins.h>
#include "timer.h"
#include "key.h"
#include "pcf8591.h"

// size of wave form array
// also used in calculating waves
#define WAVE_SIZE 32
// maximum voltage output
#define MAX_VOLT ((float)5.0) 
// default timer reloading value
#define DEFAULT_TL (65536-251)%256
#define DEFAULT_TH (65536-251)/256
// step when changing frequency, R for Rough, A for Accurate
// (Tested) Rough changes about 20Hz a time
// Accurate changes about 1Hz a time
// THE FREQUENCY CHANGING IS NON-LINEAR BUT
// THERE IS NO SOLUTION PROVIDED IN THIS ALGORITHM
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
// used in for loops
unsigned char i;
// indicates whether there is DA output
bit hasStart;
// storing keycode
unsigned char keyCode;
// initValue of Timer
unsigned int initValue;
// reloading value of TH and TL
unsigned int tlValue, thValue;
// output voltage, modified by key pressing
float voltage;
// function pointer, point to genWaves(), for brief calling
void (*generater)(void);

// Table of sin wave
unsigned char sin[] = {
    // Uncomment this line below when WAVE_SIZE==16
    // 128,176,218,245,255,245,218,176,128,79,37,10,0,10,37,79
    // Comment the lines below when WAVE_SIZE==16
    128, 152, 176, 198, 218, 234, 245, 253,
    255, 253, 245, 234, 218, 198, 176, 152,
    128, 103,  79,  57,  37,  21,  10,   2,  
      0,   2,  10,  21,  37,  57,  79, 103
};
// Table of LUT, recalculated whenever a loop in main()
volatile unsigned char waveForm[WAVE_SIZE];

/*
 * Initialization
 * This function is for global usage
 * Initialize these things:
 * Timer0, DA, P1(for showing status of program)
 * voltage, initValue(used in Timer0, for reloaing values)
 * tlValue & thValue (calculated initValue, for reloading values in Timer 0)
 * generater (function pointer, for calculating waveForm[])
 */
void init() {
    P1 = 0xfe;          // Light on LED 0
    voltage = 5.0;      // Maximum initial voltage output
	hasStart = 1;
    initValue = 251;    // (Tested) Under this value, the frequency is 100Hz
    tlValue = (65536-initValue)%256; // Reloading value of Timer
    thValue = (65536-initValue)/256; // Reloading value of Timer
    generater = genSin; // Function pointer pointing to genSin()
    generater();        // Call function pointer (genSin() here)		
    DACStart();         // Send the ADDRESS and CONTROL bytes to PCF8591
	Init_Timer0();      // Enable Interrupt
}
// main entrance
void main() {
    init();         // Initialize things
    // main loop
    while(1) {
        keyCode = mKeyScan();   // Get key press info
        switch(keyCode) {
            case KEY_NULL:      // No key pressed (or invalied key)
                break;
            case KEY_ONOFF:     // Display controlling (not used)
				hasStart = !hasStart;
                break;
            case KEY_SIN:       // Switch to sin wave
                generater = genSin; // Function pointer, ready to call
                break;
            case KEY_SQR:       // Switch to square wave
                generater = genSqr;
                break;
            case KEY_SAW:       // Switch to sawtooth wave
                generater = genSaw;
                break;
            case KEY_TRI:       // Switch to triangle wave
                generater = genTri;
                break;
            case KEY_FRE_R_ADD: // Rough add frequency
                initValue -= FREQUENCY_STEP_R; // that is, sub of delay
                break;
            case KEY_FRE_R_SUB: // Rough sub frequency
                initValue += FREQUENCY_STEP_R;
                break;
            case KEY_FRE_A_ADD: // Accurate add frequency
                initValue -= FREQUENCY_STEP_A;
                break;
            case KEY_FRE_A_SUB:
                initValue += FREQUENCY_STEP_A;
                break;
            case KEY_AMP_R_ADD: // Rough add amplitude (output voltage)
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
        if(voltage < 0) {   // Voltage range: 0~5V
            voltage = 0.1;
        }
        if(voltage > 5.0) {
            voltage = 5.0;
        }
        if(initValue < 251) { // Timer should be no faster than this 
            initValue = 251;  // or DAC would not run normally
        }
        if(initValue > 65534) { // 65535 is the maximum value of timer
            initValue = 65534;
        }
        tlValue = (65536-initValue)%256;    // Recalculate timer reloading values
        thValue = (65536-initValue)/256;
        
        generater();   // Regenerate wave form (calculates even if nothing changes)
    }
}

void onTimer0() interrupt 1 {
	TL0 = tlValue;   // Reload timer values
	TH0 = thValue;   // Reload timer values
	if (hasStart) {
	    index ++;   // Increase index, indexing next element in waveForm[]
	    if(index == WAVE_SIZE) {    // Array out range
	        index = 0;
	    }
	    /*
	     * Send next data into DAC
	     * Connection already set up by calling DACStart()
	     * According to documentation, there can be unlimited
	     * data sent to DAC
	     */
	    ContinuesSend(waveForm[index]);
	}
}

/*
 * Generating sawtooth wave
 * Sawtooth wave increases in all-along period from zero
 * For a maximum voltage(MAX_VOLT) and a 32-element array,
 * increase 256/32=8 each time
 * Result should multi (voltage/MAX_VOLT) for voltage-adjusting
 */
void genSaw() { // 32
    for(i = 0; i < WAVE_SIZE; i++) {
        waveForm[i] = i*(255/WAVE_SIZE)*(voltage/MAX_VOLT);
    }
}

/*
 * Sin wave calculating takes a lot of time so here
 * use a preset table to index sin wave forms
 * Only need to multi voltage adjusting value
 */
void genSin() {
    for(i = 0; i < WAVE_SIZE; i++) {
        waveForm[i] = sin[i]*(voltage/MAX_VOLT);
    }
}

/*
 * Square waves are quite simple to calculate
 * During the first half period, the output is maximum value
 * and on the last half period it is zero (zero ignoring voltage adjusting) 
 */
void genSqr() {
    for(i = 0; i < WAVE_SIZE/2; i++) {
        waveForm[i] = (255)*(voltage/MAX_VOLT);
    }
    for(i = WAVE_SIZE/2; i < WAVE_SIZE; i++) {
        waveForm[i] = 0;
    }
}

/*
 * Triangle waves increase in the first half period and decrease in the last
 * half period. (Tested) There is a half-WAVE_SIZE offset in the first value,
 * so use algo to compensate the offset, that is how the WAVE_SIZE/2 + comes.
 */
void genTri() {
    for(i = 0; i < WAVE_SIZE/2; i++) {
        waveForm[i] = (WAVE_SIZE/2 + i*(255/WAVE_SIZE*2))*(voltage/MAX_VOLT);
    }
    for(i = WAVE_SIZE/2; i < WAVE_SIZE; i++) {
        waveForm[i] = 255*(voltage/MAX_VOLT) - waveForm[i - WAVE_SIZE/2];
    }
}


