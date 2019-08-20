#include "main.h"
#include "..\Device\DeviceSpi.h"
//#include "..\driverlib\MSP430F5xx_6xx\flashctl.h"
/*
********************************************************************************
*                              	使用说明
*                           取消13行注释烧录一次
*                           注释13行再烧录一次
*
*
********************************************************************************
*/
//打开下面注释烧录运行，然后再注释掉再次烧录运行一次
//#define 	Lead 	1

	unsigned char a[20];
	unsigned char b[41];

#define   softReset()            PMMCTL0 = PMMPW + PMMSWPOR + (PMMCTL0 & 0x0003)

#define   RESETVECTORADDR_APP    0xFC34       //应用程序复位向量地址
#define   SectorSize         	 512
#define   infor_BootAddr         0x1800

#define   BankBAddr              0x12400
#define   BankCAddr              0x1A400
#define   BankDAddr              0x22400

unsigned long Start_Addr1 = 0xC400;
unsigned long Start_Addr2 = 0x10000;

unsigned char ReadPacket[10];

uint8_t NewVersion = 0;
uint8_t OldVersion = 0;

void Application(void);
void Update(void);
/*
// C prototype : void HexToStr(BYTE *pbDest, BYTE *pbSrc, int nLen)
// parameter(s): [OUT] pbDest - 存放目标字符串
//	[IN] pbSrc - 输入16进制数的起始地址
//	[IN] nLen - 16进制数的字节数
// return value:
// remarks : 将16进制数转化为字符串
*/
void HexToStr( char *pbDest,  char *pbSrc, int nLen)
{
	char	ddl,ddh;
	int i;
	for (i=0; i<nLen; i++)
	{
		ddh = 48 + pbSrc[i] / 16;
		ddl = 48 + pbSrc[i] % 16;
		if (ddh > 57) ddh = ddh + 7;
		if (ddl > 57) ddl = ddl + 7;
		pbDest[i*2] = ddh;
		pbDest[i*2+1] = ddl;
	}
	pbDest[nLen*2] = '\0';
}
/*******************************************************************************
*文件名     : main.c
*作用        : bootloader,引导程序
*作者        ：王伟业
*创建时间 : 2018.6.19
*******************************************************************************/
void main(void)
{
//	unsigned char a[20];
//	unsigned char b[41];
	volatile unsigned int i, j;
//	uint8_t RecTemp=0,RxTemp;
	uint8_t Tmp[9];//flash操作中间变量
	CloseWatchDog();
	InitClock();
	InitUsart2(115200);       //Debug串口
	Init_bootGPIO();
	Init_SPI2();
	/***********Test SPI flash*******************/
	Power_3V_ON;
	delay_ms(10);
	Erase_Secotr(0x00);
//	Erase_Chip();
//	Print_Str("Erase start\r\n");
//	for (i=0;i<1600;i++)
//		Erase_Secotr(i);
//	Print_Str("Erase done\r\n");
//	while(Read_StatReg()&0x01);
//	for(i=0;i<20;i++)
//		a[i]=i+1;
//	Write_Enable();
//	Write_Date(0x00,a,20);
//
//	for(i=0;i<20;i++)
//	  a[i]=0;
//	while(Read_StatReg()&0x01);
//	Read_Data(0x00,a,20);
//	for(i=0;i<20;i++)
//	{
//		if (a[i]==0)
//			break;
//	}
//	if (20 == i)
//	{
//		Print_Str("spi_flash test:\r\n");
//		HexToStr(b,a,20);
//		Print_Str(a);
//		Print_Str("\r\n");
//		Print_Str("Spi_flash Verify Success!\r\n");
//	}
//	else
//	{
//		Print_Str("Spi_flash Verify Failure!\r\n");
//	}
	/******************************/
//【出厂复位引导参数】
#ifdef Lead
	loop:
		delay_ms(10);
		Tmp[0] = 0xFF;
		Tmp[1] = 0xFF;
		FlashRsvWrite(Tmp, 2, infor_BootAddr, 0);//第一次OldVersion写入0xFF
		delay_ms(10);
		if(infoFLASH_read(0, infor_BootAddr) == 0xFF && infoFLASH_read(1, infor_BootAddr) == 0xFF)
			delay_ms(100);
		else
			goto loop;

		loop2:
			delay_ms(10);
			Tmp[0] = 0xFF;//出厂日期_年
			Tmp[1] = 0xFF;//出厂日期_月
			Tmp[2] = 0xFF;//出厂日期_日
			Tmp[3] = 0xFF;//设备编号高八位
			Tmp[4] = 0xFF;//设备编号低八位
			Tmp[5] = 0xFF;//出厂编号高八位
			Tmp[6] = 0xFF;//出厂编号低八位
			Tmp[7] = 0xFF;//出厂编号高八位
			Tmp[8] = 0xFF;//出厂编号低八位
			FlashRsvWrite(Tmp, 9, infor_ChargeAddr, 0);//把终端信息写入FLASH
			delay_ms(10);
			if(infoFLASH_read(0, infor_ChargeAddr) == 0xFF && infoFLASH_read(1, infor_ChargeAddr) == 0xFF\
			&& infoFLASH_read(2, infor_ChargeAddr) == 0xFF && infoFLASH_read(3, infor_ChargeAddr) == 0xFF\
			&& infoFLASH_read(4, infor_ChargeAddr) == 0xFF && infoFLASH_read(5, infor_ChargeAddr) == 0xFF\
			&& infoFLASH_read(6, infor_ChargeAddr) == 0xFF)
				delay_ms(100);
			else
				goto loop2;
#endif
//【出厂复位引导参数】到此结束

	OldVersion = infoFLASH_read(1,infor_BootAddr);

	if(ResetVectorValid() == Enble_App)          // 判断是否已经下载过应用程序
	{
		Application();                           // 执行应用程序
	}
	else
	{
		Update();
	}

			//test
//			ByteNum = 0;
//			j=0;
//	while(1)
//	{
//		if((UCA2IFG & UCRXIFG))//以下是用于接收AP指令并解析处理
//		{
//			RxTemp = UCA2RXBUF;
//			while(Read_StatReg()&0x01);
//			Write_Enable();
//			Write_Byte(ByteNum,RxTemp);
//			ByteNum++;
////			i++;
//			if(ByteNum%20==0)
//			{
//				while(j<ByteNum)
//				{
//					for(i=0;i<20;i++)
//						a[i]=0;
//					while(Read_StatReg()&0x01);
//					Read_Data(j,a,20);
//					j+=20;
//	//				Print_Str("spi_flash test:\r\n");
//	////				HexToStr(b,a,20);
//					SendNByteToUart2(a,20);
//				}
////				Print_Str("\r\n");
////				Print_Str("Spi_flash Verify Success!\r\n");
////				ByteNum +=5;
//
//			}
//
//
//		}
//	}
}

/*******************************************************************************
* Function Name  : Application
* Description    : 在C中调用汇编指令,实现地址转移
* Input para     : None
* Output para    : None
*******************************************************************************/
void Application(void)
{
	uint16_t i = 0;
	uint8_t Tmp[1];//flash操作中间变量
	uint8_t SendBytes[15] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
	uint8_t APP_Flag = 0;

	while(APP_Flag == 0)
	{
		if(OldVersion == 0xFF)
		{
			if((UCA2IFG & UCRXIFG))//以下是用于接收AP指令并解析处理
			{
				for(i = 0; i < 9; i++)
				{
					ReadPacket[i] = ReadPacket[i + 1];
				}
				ReadPacket[9] = UCA2RXBUF;//滑动串口数据流窗口

				if(ReadPacket[0] == 0x0D && ReadPacket[9] == 0x0D)
				{
					if(ReadPacket[1] == 0xEF)//固件升级请求指令
					{
						loop5:
							delay_ms(10);
							Tmp[0] = 0x01;
							APP_Flag = 1;
							FlashRsvWrite(Tmp, 1, infor_BootAddr, 0);//把infor_BootAddr写0x01，建立固件升级标志位
							delay_ms(10);
							if(infoFLASH_read(0, infor_BootAddr) == 0x01)
								softReset();
							else
								goto loop5;
					}
					else if(ReadPacket[1] == 0xF0)//终端信息查询指令
					{
						SendBytes[0] = 0xFF;
						SendBytes[1] = infoFLASH_read(1, infor_BootAddr);//固件版本号
						SendBytes[2] = infoFLASH_read(0, infor_ChargeAddr);//DevID_H（设备编号高位）
						SendBytes[3] = infoFLASH_read(1, infor_ChargeAddr);//DevID_L（设备编号低位）
						SendBytes[4] = infoFLASH_read(2, infor_ChargeAddr);//DevSN_H（出厂编号高位）
						SendBytes[5] = infoFLASH_read(3, infor_ChargeAddr);//DevSN_L（出厂编号低位）
						SendBytes[6] = infoFLASH_read(4, infor_ChargeAddr);//PD_H（出厂日期_年）
						SendBytes[7] = infoFLASH_read(5, infor_ChargeAddr);//PD_M（出厂日期_月）
						SendBytes[8] = infoFLASH_read(6, infor_ChargeAddr);//PD_L（出厂日期_日）
						SendBytes[9] = infoFLASH_read(7, infor_ChargeAddr);//终端类型
						SendBytes[10] = infoFLASH_read(8, infor_ChargeAddr);//通讯方式
						SendBytes[11] = infoFLASH_read(9, infor_ChargeAddr);//Deveui_H
						SendBytes[12] = infoFLASH_read(10, infor_ChargeAddr);//Deveui_L
						SendBytes[13] = infoFLASH_read(11, infor_ChargeAddr);//上传周期
						SendBytes[14] = 0xFF;
						SendNByteToUart2(SendBytes, 15);
					}
					else if(ReadPacket[1] == 0xFD)//复位终端
					{
						delay_ms(10);
						delay_ms(10);
						softReset();
					}
				}
			}
		}
		else
		{
			__disable_interrupt();    // Disable global interrupts
			asm(" br &0xFC34");
		}
	}
}
/*******************************************************************************
* Function Name  : Update
* Description    :
* Input para     : None
* Output para    : None
*******************************************************************************/
long textcount = 0;
void Update(void)
{
	uint8_t Tmp[3];//flash操作中间变量
	uint16_t i = 0, j = 0, k = 0;
    uint8_t WriterBuf[16];
    uint8_t RecBuf[50];
    uint8_t RecCnt = 0;
    uint8_t RecTemp=0,RxTemp;
    uint32_t Addr = 0x00000000;
    uint32_t Addr_Base = 0;
    uint8_t AddrFlag = 0;
    unsigned char UPDate_Flag = 0;
    unsigned char FlashWrite = 0;
    unsigned char UPDate_send_Flag = 0;

   //此处添加延时5S延时
    for(k = 0; k < 500; k++)
    	delay_ms(10);
    delay_ms(100);
    SendByteToUart2(0x75);//1.终端复位成功
    delay_ms(100);

    //以下While循环是用于与AP交互
    while(UPDate_Flag == 0)
    {
		if(UPDate_send_Flag == 0)     //此处的if判断只用于读取设备当前版本号
		{
			UPDate_send_Flag = 1;
			OldVersion =  infoFLASH_read(1,infor_BootAddr);
			delay_ms(10);
		}
		if((UCA2IFG & UCRXIFG))//以下是用于接收AP指令并解析处理
		{
			for(i = 0; i < 9; i++)
			{
				ReadPacket[i] = ReadPacket[i + 1];
			}
			ReadPacket[9] = UCA2RXBUF;//滑动串口数据流窗口

			if(ReadPacket[0] == 0x0D && ReadPacket[9] == 0x0D)
			{
				if(ReadPacket[1] == 0xEF)
				{
					NewVersion = ReadPacket[2];//获取新版本
					if(NewVersion != OldVersion && NewVersion != 0xFF)
					{
						delay_ms(100);
						SendByteToUart2(0x77);//2.固件版本校验成功
						delay_ms(100);
						UPDate_Flag = 1;//置位，退出大循环,准备升级
					}
					else
					{
						loop2:
							delay_ms(100);
							Tmp[0] = 0xFF;
							FlashRsvWrite(Tmp, 1, infor_BootAddr, 0);//把infor_BootAddr写0xFF，清除固件升级标志位
							delay_ms(10);
							if(infoFLASH_read(0, infor_BootAddr) == 0xFF)
								delay_ms(100);
							else
								goto loop2;
							delay_ms(100);
							SendByteToUart2(0x76);   //[ERR03]固件版本校验未通过：版本号相同或为0xFF
							delay_ms(100);
							delay_ms(100);
							Application();              //固件版本相同，不升级，进入应用程序
					}
				}
			}
		}
    }
    //以下是擦除代码区并且开始接收固件数据
    if(UPDate_Flag == 1)
    {
    	UPDate_Flag = 0;
    	delay_ms(100);
    	SendByteToUart2(0x78);//3.开始擦除FLASH
    	delay_ms(100);
        /* 在此处根据和上位机软件协议添加擦除flash的程序*/
/*        for(i=0;i<29;i++)//擦除指定FLASH区域
        {
        	FlashCtl_eraseSegment((uint8_t *)Start_Addr1);//修改FLASH地址
        	delay_us(10);
        	Start_Addr1 += 0x200;
        	if(Start_Addr1==0xFE00)//当擦到最后一个扇区时
        	{
        		Start_Addr1 = 0xC400;//将指针移到开头
        	}
        }
        for(i=0;i<18;i++)
        {
            FlashCtl_eraseSegment((uint8_t *)Start_Addr2);//修改FLASH地址
            delay_us(10);
            Start_Addr2 += 0x200;
            if(Start_Addr2==0x12400)//当擦到最后一个扇区时
            {
                Start_Addr2 = 0x10000;//将指针移到开头
            }
        }
        FlashCtl_eraseBank((uint8_t *)BankBAddr);
        delay_us(100);
        FlashCtl_eraseBank((uint8_t *)BankCAddr);
        delay_us(100);
        FlashCtl_eraseBank((uint8_t *)BankDAddr);
*/
    	/************改为擦出SPIFlash********************/
    	Power_3V_ON;
		delay_ms(10);
//		Print_Str("Erase start\r\n");
		readAddr = 0;
		for (i=0;i<6;i++)
		{
			Erase_Block(readAddr);
			readAddr += 0x10000;
			while(Read_StatReg()&0x01);
					Read_Byte(0);	//擦除后需要先读一次
		}
		readAddr = 0;
//		Erase_Chip();



//		Print_Str("Erase done\r\n");

        delay_us(100);
    	delay_ms(100);
        SendByteToUart2(0x79);//4.成功擦除FLASH
        delay_ms(100);delay_ms(100);
        SendByteToUart2(0x7A);//5.开始传输固件
        Write_Enable();
        i=0;
        /*下面一段函数为串口数据接收和处理程序 */
        while(1)
        {
        	if((UCA2IFG & UCRXIFG))//以下是用于接收AP指令并解析处理
        	{
        		RxTemp = UCA2RXBUF;
				while(Read_StatReg()&0x01);
				Write_Enable();
				Write_Byte(ByteNum,RxTemp);
				ByteNum++;
        		if(RxTemp == 'q')
        		{
        			FlashWrite = 1;
        			Print_Str("Transfer finished,writing MCU Flash...\r\n");
//        			while(ByteNum>20)
//        			{
//						for(i=0;i<20;i++)
//						  a[i]=0;
//						while(Read_StatReg()&0x01);
//						Read_Data(readAddr,a,20);
//						SendNByteToUart2(a,20);
//						ByteNum-=20;
//						readAddr+=20;
//        			}
//
//        			for(i=0;i<ByteNum;i++)
//					  a[i]=0;
//					while(Read_StatReg()&0x01);
//					Read_Data(readAddr,a,ByteNum);
//					SendNByteToUart2(a,ByteNum);
//					while(1);
        		}
        	}//end of if((UCA2IFG & UCRXIFG))//以下是用于接收AP指令并解析处理
        	if(FlashWrite ==1)
        	{
        		/* 在此处根据和上位机软件协议添加擦除flash的程序*/
		        for(i=0;i<29;i++)//擦除指定FLASH区域
				{
					FlashCtl_eraseSegment((uint8_t *)Start_Addr1);//修改FLASH地址
					delay_us(10);
					Start_Addr1 += 0x200;
					if(Start_Addr1==0xFE00)//当擦到最后一个扇区时
					{
						Start_Addr1 = 0xC400;//将指针移到开头
					}
				}
				for(i=0;i<18;i++)
				{
					FlashCtl_eraseSegment((uint8_t *)Start_Addr2);//修改FLASH地址
					delay_us(10);
					Start_Addr2 += 0x200;
					if(Start_Addr2==0x12400)//当擦到最后一个扇区时
					{
						Start_Addr2 = 0x10000;//将指针移到开头
					}
				}
				FlashCtl_eraseBank((uint8_t *)BankBAddr);
				delay_us(100);
				FlashCtl_eraseBank((uint8_t *)BankCAddr);
				delay_us(100);
				FlashCtl_eraseBank((uint8_t *)BankDAddr);
				while(readAddr <= ByteNum)
				{
					RxTemp = Read_Byte(readAddr++);
					switch(RxTemp)
					{
						case '@':
							AddrFlag = 1;
							break;
						case 'q':
							{
								loop3:
									delay_ms(100);
									Tmp[0] = 0xFF;//把infor_BootAddr写0xFF，清除固件升级标志位
									Tmp[1] = NewVersion;//infoFLASH_read(2, infor_BootAddr);//将新的版本号写入infor_BootAddr
									FlashRsvWrite(Tmp, 2, infor_BootAddr, 0);
									delay_ms(100);
									if(infoFLASH_read(0, infor_BootAddr) == 0xFF && infoFLASH_read(1, infor_BootAddr) == NewVersion)
									{
										delay_ms(100);
										SendByteToUart2(0x7B);//6.固件传输完成
										delay_ms(100);
										softReset();
									}
									else
										goto loop3;
							}
							break;
						default:
							break;
					}
					if(AddrFlag)//解析@后的地址:[地址一行用字符发送]
					{
						RecBuf[RecCnt++] = RxTemp;
						if(RxTemp == 0x0A)//识别到换行
						{
							AddrFlag = 0;
							RecCnt = 0;
							Addr_Base = Addr & 0xFFFFFF;
							Addr = 0;
						}
						else if(RecCnt > 1)
						{
							Addr <<= 4;
							Addr += asciitohex(RxTemp);
						}
					}
					else//解析数据
					{
						if(RxTemp >= '0')
						{
							RecBuf[RecCnt++]=RxTemp;
							RecTemp<<=4;
							RecTemp+=ASCIIToHex(RecBuf[RecCnt-1]);
						}
						if(RecCnt==2)
						{
							RecCnt = 0;
							WriterBuf[j++] = RecTemp;
							if(j == 1)
							{
								j = 0;
								//存储，HEX条件下，改为每个Byte存储一次，否则固件长度为奇数会出错
								FlashCtl_write8(WriterBuf,(unsigned char *)Addr_Base,1);
								Addr_Base += 1;
							}
						}
					}
				}
        	}
        }
    }
}

//描述: 中断向量列表
#pragma vector=PORT6_VECTOR
__interrupt void PORT6_VECTOR_ISR(void)

{
	asm(" br &0xFC00");
}

#pragma vector=RTC_VECTOR

__interrupt void RTC_VECTOR_ISR(void)
{
	asm(" br &0xFC02");
}

#pragma vector=PORT2_VECTOR
__interrupt void PORT2_VECTOR_ISR(void)
{
	asm(" br &0xFC04");
}

#pragma vector=TIMER2_A1_VECTOR
__interrupt void TIMER2_A1_VECTOR_ISR(void)
{
	asm(" br &0xFC06");
}

#pragma vector=TIMER2_A0_VECTOR
__interrupt void TIMER2_A0_VECTOR_ISR(void)
{
	asm(" br &0xFC08");
}

#pragma vector=PORT1_VECTOR
__interrupt void PORT1_VECTOR_ISR(void)
{
	asm(" br &0xFC0A");
}

#pragma vector=TIMER1_A1_VECTOR
__interrupt void TIMER1_A1_VECTOR_ISR(void)
{
	asm(" br &0xFC0C");
}

#pragma vector=TIMER1_A0_VECTOR
__interrupt void TIMER1_A0_VECTOR_ISR(void)

{
	asm(" br &0xFC0E");
}

#pragma vector=TIMER0_B1_VECTOR
__interrupt void TIMER0_B1_VECTOR_ISR(void)

{
	asm(" br &0xFC10");
}


#pragma vector=TIMER0_B0_VECTOR
__interrupt void TIMER0_B0_VECTOR_ISR(void)
{
	asm(" br &0xFC12");
}

#pragma vector=USCI_B3_VECTOR
__interrupt void USCI_B3_VECTOR_ISR(void)

{
	asm(" br &0xFC14");
}

#pragma vector=USCI_A3_VECTOR
__interrupt void USCI_A3_VECTOR_ISR(void)

{
	asm(" br &0xFC16");
}

#pragma vector=DMA_VECTOR
__interrupt void DMA_VECTOR_ISR(void)
{
	asm(" br &0xFC18");
}

#pragma vector=TIMER0_A1_VECTOR
__interrupt void TIMER0_A1_VECTOR_ISR(void)
{
	asm(" br &0xFC1A");
}

#pragma vector=TIMER0_A0_VECTOR
__interrupt void TIMER0_A0_VECTOR_ISR(void)
{
	asm(" br &0xFC1C");
}

#pragma vector=USCI_B2_VECTOR
__interrupt void USCI_B2_VECTOR_ISR(void)
{
	asm(" br &0xFC1E");
}

#pragma vector=USCI_A2_VECTOR
__interrupt void USCI_A2_VECTOR_ISR(void)
{
	asm(" br &0xFC20");
}

#pragma vector=ADC10_VECTOR

__interrupt void ADC10_VECTOR_ISR(void)
{
	asm(" br &0xFC22");
}

#pragma vector=USCI_B1_VECTOR
__interrupt void USCI_B1_VECTOR_ISR(void)
{
	asm(" br &0xFC24");
}

#pragma vector=USCI_A1_VECTOR
__interrupt void USCI_A1_VECTOR_ISR(void)

{
	asm(" br &0xFC26");
}

#pragma vector=WDT_VECTOR
__interrupt void WDT_VECTOR_ISR(void)

{
	asm(" br &0xFC28");
}

#pragma vector=USCI_B0_VECTOR
__interrupt void USCI_B0_VECTOR_ISR(void)

{
	asm(" br &0xFC2A");
}

#pragma vector=USCI_A0_VECTOR
__interrupt void USCI_A0_VECTOR_ISR(void)

{
	asm(" br &0xFC2C");
}

#pragma vector=COMP_B_VECTOR

__interrupt void COMP_B_VECTOR_ISR(void)
{
	asm(" br &0xFC2E");
}

#pragma vector=UNMI_VECTOR
__interrupt void UNMI_VECTOR_ISR(void)

{
	asm(" br &0xFC30");
}

#pragma vector=SYSNMI_VECTOR
__interrupt void SYSNMI_VECTOR_ISR(void)
{
	asm(" br &0xFC32");
}
