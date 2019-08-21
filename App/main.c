#include "main.h"
#include "..\Device\DeviceSpi.h"
//#include "..\driverlib\MSP430F5xx_6xx\flashctl.h"
/*
********************************************************************************
*                              	ʹ��˵��
*                           ȡ��13��ע����¼һ��
*                           ע��13������¼һ��
*
*
********************************************************************************
*/
//������ע����¼���У�Ȼ����ע�͵��ٴ���¼����һ��
//#define 	Lead 	1

	unsigned char a[20];
	unsigned char b[41];

#define   softReset()            PMMCTL0 = PMMPW + PMMSWPOR + (PMMCTL0 & 0x0003)

#define   RESETVECTORADDR_APP    0xFC34       //Ӧ�ó���λ������ַ
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
// parameter(s): [OUT] pbDest - ���Ŀ���ַ���
//	[IN] pbSrc - ����16����������ʼ��ַ
//	[IN] nLen - 16���������ֽ���
// return value:
// remarks : ��16������ת��Ϊ�ַ���
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
*�ļ���     : main.c
*����        : bootloader,��������
*����        ����ΰҵ
*����ʱ�� : 2018.6.19
*******************************************************************************/
void main(void)
{
//	unsigned char a[20];
//	unsigned char b[41];
	volatile unsigned int i, j;
	uint8_t Flash_temp;
	// uint8_t Data_temp[2];
//	uint8_t RecTemp=0,RxTemp;
	uint8_t Tmp[9];//flash�����м����
	CloseWatchDog();
	InitClock();
	InitUsart2(115200);       //Debug����
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
//��������λ����������
#ifdef Lead
	loop:
		delay_ms(10);
		Tmp[0] = 0xFF;
		Tmp[1] = 0xFF;
		FlashRsvWrite(Tmp, 2, infor_BootAddr, 0);//��һ��OldVersionд��0xFF
		delay_ms(10);
		if(infoFLASH_read(0, infor_BootAddr) == 0xFF && infoFLASH_read(1, infor_BootAddr) == 0xFF)
			delay_ms(100);
		else
			goto loop;

		loop2:
			delay_ms(10);
			Tmp[0] = 0xFF;//��������_��
			Tmp[1] = 0xFF;//��������_��
			Tmp[2] = 0xFF;//��������_��
			Tmp[3] = 0xFF;//�豸��Ÿ߰�λ
			Tmp[4] = 0xFF;//�豸��ŵͰ�λ
			Tmp[5] = 0xFF;//������Ÿ߰�λ
			Tmp[6] = 0xFF;//������ŵͰ�λ
			Tmp[7] = 0xFF;//������Ÿ߰�λ
			Tmp[8] = 0xFF;//������ŵͰ�λ
			FlashRsvWrite(Tmp, 9, infor_ChargeAddr, 0);//���ն���Ϣд��FLASH
			delay_ms(10);
			if(infoFLASH_read(0, infor_ChargeAddr) == 0xFF && infoFLASH_read(1, infor_ChargeAddr) == 0xFF\
			&& infoFLASH_read(2, infor_ChargeAddr) == 0xFF && infoFLASH_read(3, infor_ChargeAddr) == 0xFF\
			&& infoFLASH_read(4, infor_ChargeAddr) == 0xFF && infoFLASH_read(5, infor_ChargeAddr) == 0xFF\
			&& infoFLASH_read(6, infor_ChargeAddr) == 0xFF)
				delay_ms(100);
			else
				goto loop2;
#endif
//��������λ�������������˽���

	OldVersion = infoFLASH_read(1,infor_BootAddr);

	if(ResetVectorValid() == Enble_App)          // �ж��Ƿ��Ѿ����ع�Ӧ�ó���
	{
		for(i=0;i<10;i++)
		{
			Flash_temp = infoFLASH_read(i,Start_Addr1);
			if(Flash_temp != 0xFF)
				break;
		}
		if(i < 10)
			Application();                           // ִ��Ӧ�ó���
		else
		{
			Tmp[0] = 0x01;				//force to update
			Tmp[1] = 0xFF;				//version set to 0xFF
			delay_ms(100);
			FlashRsvWrite(Tmp,2,infor_BootAddr,0);
			//FlashRsvWrite(infor_BootAddr,0xFF);
			delay_ms(100);
			softReset();
		}
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
//		if((UCA2IFG & UCRXIFG))//���������ڽ���APָ���������
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
* Description    : ��C�е��û��ָ��,ʵ�ֵ�ַת��
* Input para     : None
* Output para    : None
*******************************************************************************/
void Application(void)
{
	uint16_t i = 0;
	uint8_t Tmp[1];//flash�����м����
	uint8_t SendBytes[15] = {0xFF, 0xFF, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0xFF};
	uint8_t APP_Flag = 0;

	while(APP_Flag == 0)
	{
		if(OldVersion == 0xFF)
		{
			if((UCA2IFG & UCRXIFG))//���������ڽ���APָ���������
			{
				for(i = 0; i < 9; i++)
				{
					ReadPacket[i] = ReadPacket[i + 1];
				}
				ReadPacket[9] = UCA2RXBUF;//������������������

				if(ReadPacket[0] == 0x0D && ReadPacket[9] == 0x0D)
				{
					if(ReadPacket[1] == 0xEF)//�̼���������ָ��
					{
						loop5:
							delay_ms(10);
							Tmp[0] = 0x01;
							APP_Flag = 1;
							FlashRsvWrite(Tmp, 1, infor_BootAddr, 0);//��infor_BootAddrд0x01�������̼�������־λ
							delay_ms(10);
							if(infoFLASH_read(0, infor_BootAddr) == 0x01)
								softReset();
							else
								goto loop5;
					}
					else if(ReadPacket[1] == 0xF0)//�ն���Ϣ��ѯָ��
					{
						SendBytes[0] = 0xFF;
						SendBytes[1] = infoFLASH_read(1, infor_BootAddr);//�̼��汾��
						SendBytes[2] = infoFLASH_read(0, infor_ChargeAddr);//DevID_H���豸��Ÿ�λ��
						SendBytes[3] = infoFLASH_read(1, infor_ChargeAddr);//DevID_L���豸��ŵ�λ��
						SendBytes[4] = infoFLASH_read(2, infor_ChargeAddr);//DevSN_H��������Ÿ�λ��
						SendBytes[5] = infoFLASH_read(3, infor_ChargeAddr);//DevSN_L��������ŵ�λ��
						SendBytes[6] = infoFLASH_read(4, infor_ChargeAddr);//PD_H����������_�꣩
						SendBytes[7] = infoFLASH_read(5, infor_ChargeAddr);//PD_M����������_�£�
						SendBytes[8] = infoFLASH_read(6, infor_ChargeAddr);//PD_L����������_�գ�
						SendBytes[9] = infoFLASH_read(7, infor_ChargeAddr);//�ն�����
						SendBytes[10] = infoFLASH_read(8, infor_ChargeAddr);//ͨѶ��ʽ
						SendBytes[11] = infoFLASH_read(9, infor_ChargeAddr);//Deveui_H
						SendBytes[12] = infoFLASH_read(10, infor_ChargeAddr);//Deveui_L
						SendBytes[13] = infoFLASH_read(11, infor_ChargeAddr);//�ϴ�����
						SendBytes[14] = 0xFF;
						SendNByteToUart2(SendBytes, 15);
					}
					else if(ReadPacket[1] == 0xFD)//��λ�ն�
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
	uint8_t Tmp[3];//flash�����м����
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

   //5s延时
    for(k = 0; k < 500; k++)
    	delay_ms(10);
    delay_ms(100);
    SendByteToUart2(0x75);//清除txtlog
    delay_ms(100);

    //����Whileѭ����������AP����
    while(UPDate_Flag == 0)
    {
		if(UPDate_send_Flag == 0)     //�˴���if�ж�ֻ���ڶ�ȡ�豸��ǰ�汾��
		{
			UPDate_send_Flag = 1;
			OldVersion =  infoFLASH_read(1,infor_BootAddr);
			delay_ms(10);
		}
		if((UCA2IFG & UCRXIFG))//���������ڽ���APָ���������
		{
			for(i = 0; i < 9; i++)
			{
				ReadPacket[i] = ReadPacket[i + 1];
			}
			ReadPacket[9] = UCA2RXBUF;//������������������

			if(ReadPacket[0] == 0x0D && ReadPacket[9] == 0x0D)
			{
				if(ReadPacket[1] == 0xEF)
				{
					NewVersion = ReadPacket[2];//��ȡ�°汾
					if(NewVersion != OldVersion && NewVersion != 0xFF)
					{
						delay_ms(100);
						SendByteToUart2(0x77);//2.�̼��汾У��ɹ�
						delay_ms(100);
						UPDate_Flag = 1;//��λ���˳���ѭ��,׼������
					}
					else
					{
						loop2:
							delay_ms(100);
							Tmp[0] = 0xFF;
							FlashRsvWrite(Tmp, 1, infor_BootAddr, 0);//��infor_BootAddrд0xFF������̼�������־λ
							delay_ms(10);
							if(infoFLASH_read(0, infor_BootAddr) == 0xFF)
								delay_ms(100);
							else
								goto loop2;
							delay_ms(100);
							SendByteToUart2(0x76);   //[ERR03]�̼��汾У��δͨ�����汾����ͬ��Ϊ0xFF
							delay_ms(100);
							delay_ms(100);
							Application();              //�̼��汾��ͬ��������������Ӧ�ó���
					}
				}
			}
		}
    }
    //�����ǲ������������ҿ�ʼ���չ̼�����
    if(UPDate_Flag == 1)
    {
    	UPDate_Flag = 0;
    	delay_ms(100);
    	SendByteToUart2(0x78);//3.��ʼ����FLASH
    	delay_ms(100);
        /* �ڴ˴����ݺ���λ�����Э����Ӳ���flash�ĳ���*/
/*        for(i=0;i<29;i++)//����ָ��FLASH����
        {
        	FlashCtl_eraseSegment((uint8_t *)Start_Addr1);//�޸�FLASH��ַ
        	delay_us(10);
        	Start_Addr1 += 0x200;
        	if(Start_Addr1==0xFE00)//���������һ������ʱ
        	{
        		Start_Addr1 = 0xC400;//��ָ���Ƶ���ͷ
        	}
        }
        for(i=0;i<18;i++)
        {
            FlashCtl_eraseSegment((uint8_t *)Start_Addr2);//�޸�FLASH��ַ
            delay_us(10);
            Start_Addr2 += 0x200;
            if(Start_Addr2==0x12400)//���������һ������ʱ
            {
                Start_Addr2 = 0x10000;//��ָ���Ƶ���ͷ
            }
        }
        FlashCtl_eraseBank((uint8_t *)BankBAddr);
        delay_us(100);
        FlashCtl_eraseBank((uint8_t *)BankCAddr);
        delay_us(100);
        FlashCtl_eraseBank((uint8_t *)BankDAddr);
*/
    	/************��Ϊ����SPIFlash********************/
    	Power_3V_ON;
		delay_ms(10);
//		Print_Str("Erase start\r\n");
		readAddr = 0;
		for (i=0;i<6;i++)
		{
			Erase_Block(readAddr);
			readAddr += 0x10000;
			while(Read_StatReg()&0x01);
					Read_Byte(0);	//��������Ҫ�ȶ�һ��
		}
		readAddr = 0;
//		Erase_Chip();



//		Print_Str("Erase done\r\n");

        delay_us(100);
    	delay_ms(100);
        SendByteToUart2(0x79);//4.�ɹ�����FLASH
        delay_ms(100);delay_ms(100);
        SendByteToUart2(0x7A);//5.��ʼ����̼�
        Write_Enable();
        i=0;
        /*����һ�κ���Ϊ�������ݽ��պʹ������ */
        while(1)
        {
        	if((UCA2IFG & UCRXIFG))//���������ڽ���APָ���������
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
        	}//end of if((UCA2IFG & UCRXIFG))//���������ڽ���APָ���������
        	if(FlashWrite ==1)
        	{
        		/* �ڴ˴����ݺ���λ�����Э����Ӳ���flash�ĳ���*/
		        for(i=0;i<29;i++)//����ָ��FLASH����
				{
					FlashCtl_eraseSegment((uint8_t *)Start_Addr1);//�޸�FLASH��ַ
					delay_us(10);
					Start_Addr1 += 0x200;
					if(Start_Addr1==0xFE00)//���������һ������ʱ
					{
						Start_Addr1 = 0xC400;//��ָ���Ƶ���ͷ
					}
				}
				for(i=0;i<18;i++)
				{
					FlashCtl_eraseSegment((uint8_t *)Start_Addr2);//�޸�FLASH��ַ
					delay_us(10);
					Start_Addr2 += 0x200;
					if(Start_Addr2==0x12400)//���������һ������ʱ
					{
						Start_Addr2 = 0x10000;//��ָ���Ƶ���ͷ
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
									Tmp[0] = 0xFF;//��infor_BootAddrд0xFF������̼�������־λ
									Tmp[1] = NewVersion;//infoFLASH_read(2, infor_BootAddr);//���µİ汾��д��infor_BootAddr
									FlashRsvWrite(Tmp, 2, infor_BootAddr, 0);
									delay_ms(100);
									if(infoFLASH_read(0, infor_BootAddr) == 0xFF && infoFLASH_read(1, infor_BootAddr) == NewVersion)
									{
										delay_ms(100);
										SendByteToUart2(0x7B);//6.�̼��������
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
					if(AddrFlag)//����@��ĵ�ַ:[��ַһ�����ַ�����]
					{
						RecBuf[RecCnt++] = RxTemp;
						if(RxTemp == 0x0A)//ʶ�𵽻���
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
					else//��������
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
								//�洢��HEX�����£���Ϊÿ��Byte�洢һ�Σ�����̼�����Ϊ���������
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

//����: �ж������б�
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
