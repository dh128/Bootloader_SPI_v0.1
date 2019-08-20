#include "..\..\App\main.h"

unsigned char Uart_2_Flag=0;		//串口接受完，在接受中断会置0，接受完1s后定时内置1

const uint8_t USCIModulation[] = {0x00,0x10,0x20,0x30,0x40,0x50,0x60,0x70,0x80,0x90,
		                          0xA0,0xB0,0xC0,0xD0,0xE0,0xF0};
unsigned char cRxBuff[cRxLength];		//UART2 receive data buff
unsigned char cRxNum=0;		            //UART2 receive data num
/*******************************************************************************
* Function Name  : Init_Usart2		Used for Debug
* Description    : BaudRate115200bps
* Input para     : None
* Output para    : None
*******************************************************************************/
void InitUsart2(uint32_t BaudRate)
{
	float MidValue;
	uint32_t Mainclk;
	uint32_t tem;
	uint8_t UCBRF;

	Mainclk = MCLK_FREQ;

	P7SEL |= BIT0+BIT1;                       // P7.0,1 = USCI_A1 TXD/RXD
    UCA2CTL1 |= UCSWRST;                      // 复位设备状态
    UCA2CTL1 |= UCSSEL_2;                     // 选择参考时钟为SCMLK=16MHz

    MidValue = (float)Mainclk/16/BaudRate;
    tem = Mainclk/16/BaudRate;

    UCBRF = (MidValue- tem)*16 + 1;
    UCBRF = USCIModulation[UCBRF];

    UCA2BR0 = tem;
    UCA2BR1 = 0;                              //
	UCA2MCTL |= UCBRS_0 + UCBRF + UCOS16;   // Modulation UCBRSx=1, UCBRFx=3

    UCA2CTL1 &= ~UCSWRST;                     // 完成USCI初始化配置
    //UCA2IE |= UCRXIE;                         // 使能接收中断

//    am_util_stdio_printf_init((am_util_stdio_print_char_t)SendStringToUart2); //串口打印映射到串口0
}
/*******************************************************************************
* Function Name  : SendByteToUart2
* Description    :
* Input para     : data,len
* Output para    : None
*******************************************************************************/
void SendByteToUart2(unsigned char Chr)
{
	UCA2TXBUF = Chr;
	while(!(UCA2IFG & UCTXIFG));
}
/*******************************************************************************
* Function Name  : SendNByteToUart2
* Description    :
* Input para     : data,len
* Output para    : None
*******************************************************************************/
void SendNByteToUart2(unsigned char *data,unsigned char len)
{
	static unsigned char m = 0;
	for(m=0;m<len;m++)
	{
		UCA2TXBUF = data[m];
		while(!(UCA2IFG & UCTXIFG));
	}
}
/*******************************************************************************
* Function Name  : Print_Str
* Description    :
* Input para     : *s
* Output para    : None
*******************************************************************************/
void Print_Str(unsigned char *s)
{
	while(*s != '\0')
	{
		SendByteToUart2(*s++);
	}

}
/*******************************************************************************
* Function Name  : SendStringToUart2
* Description    :
* Input para     : *s
* Output para    : None
*******************************************************************************/
void SendStringToUart2(unsigned char *s)
{
	while(*s != '\0')
	{
		SendByteToUart2(*s++);
	}
}
/*******************************************************************************
* Function Name  : ASCIIToHex
* Description    :
* Input para     : cNum
* Output para    : cNum
*******************************************************************************/
uint8_t ASCIIToHex(uint8_t cNum)
{
	if(cNum>='0'&&cNum<='9')
	{
		cNum -= '0';
	}
	else if(cNum>='A'&&cNum<='F')
	{
		cNum -= 'A';
		cNum += 10;
	}

	return cNum;
}

//转小写abcdef
uint8_t asciitohex(uint8_t cNum)
{
	if(cNum>='0'&&cNum<='9')
	{
		cNum -= '0';
	}
	else if(cNum>='a'&&cNum<='f')
	{
		cNum -= 'a';
		cNum += 10;
	}

	return cNum;
}
