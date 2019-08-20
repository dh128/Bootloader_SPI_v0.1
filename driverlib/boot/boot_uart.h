/*******************************************************************************
*    File Name:  DeviceUart.h
*    Revision:  1.0
*    Description:  串口程序头文件
* *****************************************************************************/
#ifndef BOOT_UART_H
#define BOOT_UART_H

extern unsigned char Uart_2_Flag;

#define cRxLength 20		//UART2 buff length

extern unsigned char cRxBuff[];
extern unsigned char cRxNum;

void InitUsart2(uint32_t BaudRate);     //Debug
void SendByteToUart2(unsigned char Chr);
void SendNByteToUart2(unsigned char *data,unsigned char len);
void SendStringToUart2(unsigned char *s);
void Print_Str(unsigned char *s);
uint8_t ASCIIToHex(uint8_t cNum);
uint8_t asciitohex(uint8_t cNum);

#endif
