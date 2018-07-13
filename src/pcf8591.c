/**********************************************************************
@Author: frank.xin
@Date  : 7/11/2018
File name : pcf8591.c
File target: STC89C52 with PCF8591 ADDA Converter
Approach  : Control-sequence of PCF8591 converter
Sending sequence of I2C
Data on sda is sent at negedge of scl
http://frank.xin
**********************************************************************/
#include <reg52.h>  
#include "pcf8591.h"
#include <intrins.h>

#define uchar unsigned char
#define uint  unsigned int
#define AddWr 0x90   // ADDRESS BYTE


sbit sda = P2^0;      //I2C Data pin
sbit scl = P2^1;      //I2C Clock pin

// Start I2C bus by sending an UP-DOWN singal
void Start(void)
{
    sda=1;
    _nop_();
    scl=1;
    _nop_();
    sda=0;
    _nop_();
    scl=0;
}

// Acknoledge signal, that is, a zero bit
void Ack(void)
{
    sda=0;
    _nop_();
    scl=1;
    _nop_();
    scl=0;
    _nop_();
}

// Send a byte
void Send(unsigned char Data)
{ 
    unsigned char bitCounter = 8;
    unsigned char temp;

    do {
        temp = Data;    // Send a byte bit by bit
        scl = 0;        // Pull down scl pin to put data and hold
        _nop_();
        if((temp&0x80) == 0x80) {// MSB == 1 ?
            sda = 1;
        } else {
            sda = 0;
        }
        scl = 1;        // Hold for long till next falling edge
        temp = Data << 1; // move left for a next MSB
        Data = temp;
        bitCounter--;
    } while(bitCounter);
    scl = 0;    // The last bit in falling edge
}

// Start DAC by sending ADDRESS and CONTROL bytes
void DACStart() {
    Start();
    Send(AddWr);    // Send ADDRESS
    Ack();
    Send(0x40);     // Send CONTROL
    Ack();
}

// Combined with DACStart(), alway call after DACStart() is called
// Send a byte of data
void ContinuesSend(unsigned char Data) {
    Send(Data);
    Ack();
}