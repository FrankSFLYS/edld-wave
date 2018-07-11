/********引用的头文件*******/
#include <reg52.h>   //开发51单片机必须包含的头文件,该文件定义了89c52单片机内部寄存器的地址
#include "pcf8591.h" //pcf8591.h文件声明了pcf8591.c文件里边的操作函数,如果主函数要调用操作函数,则需包含此头文件
#include <intrins.h> //包含NOP空指令函数_nop_();
/***********宏定义**********/
#define uchar unsigned char
#define uint  unsigned int
#define AddWr 0x90   //写数据地址 
#define AddRd 0x91   //读数据地址

/********硬件接口定义*******/
sbit     Sda=P2^0;      //I2C  时钟 
sbit     Scl=P2^1;      //I2C  数据 

/*------------------------------------------------
                    启动IIC总线
------------------------------------------------*/
void Start(void)
{
    Sda=1;
    _nop_();
    Scl=1;
    _nop_();
    Sda=0;
    _nop_();
    Scl=0;
}

/*------------------------------------------------
                   应答IIC总线
------------------------------------------------*/
void Ack(void)
{
    Sda=0;
    _nop_();
    Scl=1;
    _nop_();
    Scl=0;
    _nop_();
}

/*------------------------------------------------
              发送一个字节
------------------------------------------------*/
void Send(unsigned char Data)
{ 
    unsigned char BitCounter=8;
    unsigned char temp;

    do {
        temp=Data;
        Scl=0;
        _nop_();
        if((temp&0x80)==0x80)
            Sda=1;
        else
            Sda=0;

        Scl=1;
        temp=Data<<1;
        Data=temp;
        BitCounter--;
    } while(BitCounter);
    Scl=0;
}

void DACStart() {
    Start();
    Send(AddWr); //写入芯片地址
    Ack();
    Send(0x40);  //写入控制位，使能DAC输出
    Ack();
}

void ContinuesSend(unsigned char Data) {
    Send(Data);
    Ack();
}