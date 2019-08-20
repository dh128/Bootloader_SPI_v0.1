#include "..\..\App\main.h"

void Init_bootGPIO(void)
{	//Initialization of ports (all unused pins as outputs with low-level)
	P1OUT = 0x00;
	P1DIR = 0xFF;
	P2OUT = 0x00;
	P2DIR = 0xFF;
	P3OUT = 0x00;
	P3DIR = 0xFF;
	P4OUT = 0x04;
	P4DIR = 0xFF;        //1:输出模式  0：输入模式
	P5OUT = 0x00;
	P5DIR = 0xFF;
	P6OUT = 0x00;
	P6DIR = 0xFF;
}
