
#include "..\App\main.h"
/*******************************************************************************
* Function Name  : SPIx_Init
* Description    :
* Input para     : None
* Output para    : None
*******************************************************************************/
void SPIx_Init(void)
{
	P7SEL |= BIT2+BIT3+BIT4;				// Set P7.2-4 as SPI peripheral

	P7DIR |= BIT2+BIT3+BIT5;
	SD_CS_High();

	UCB2CTL1 |= UCSWRST;               		          // Enable SW reset
	UCB2CTL0 |= UCCKPH+UCMSB+UCMST+UCSYNC;            //+UCCKPL;  //3��SPIģʽ����UCxSTE=1ʱ�ӻ�ʹ��
	                                                  //UCCKPH(SD CARD��Ҫ�������ض�д������UCCKPL==0��
													  //8λ����SPI���������״̬Ϊ�ߵ�ƽ����λ��ǰ
	UCB2CTL1 |= UCSSEL__SMCLK;                        // ѡ��ο�ʱ��ΪSCMLK=16MHz
	UCB2BR0 = 6;								      //6��Ƶ
	UCB2BR1 = 0;
	UCB2CTL1 &= ~UCSWRST;						      //��ɼĴ�������
	//UCB2IE |= UCRXIE;							      //ʹ���ж�

}

/*******************************************************************************
* ������	: SPIx_ReadWriteByte
* ����	: SPI��д����
* �������  : TxData
* ���ز���  : RXBUF1
*******************************************************************************/
unsigned char SPIx_ReadWriteByte(unsigned char TxData)
{
	unsigned char delayVar;

	while(!(UCB2IFG & UCTXIFG));
	UCB2TXBUF = TxData;        // Send the data sitting at the pointer DATA to the TX Buffer
	while(!(UCB2IFG & UCRXIFG));
	delayVar = UCB2RXBUF;

	return delayVar;
}
/*******************************************************************************
* ������	: SPI_B2_Init
* ����:    SPI_B2��ʼ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void Init_SPI2(void)
{
	P7SEL |= BIT2+BIT3+BIT4;				    // Set P7.2-4 as SPI peripheral
	P7DIR |= BIT2+BIT3+BIT5;

//	SD_CS_High();                               //ѡͨSD��

	UCB2CTL1 |= UCSWRST;               		    // Enable SW reset
	UCB2CTL0 |= UCCKPH+UCMSB+UCMST+UCSYNC;      //+UCCKPL;//3��SPIģʽ����UCxSTE=1ʱ�ӻ�ʹ��
	                                            //UCCKPH(SD CARD��Ҫ�������ض�д������UCCKPL==0��
											    //8λ����SPI���������״̬Ϊ�ߵ�ƽ����λ��ǰ
	UCB2CTL1 |= UCSSEL__SMCLK;                  // ѡ��ο�ʱ��ΪSCMLK=16MHz
	UCB2BR0 = 0;								//6��Ƶ
	UCB2BR1 = 0;
	UCB2CTL1 &= ~UCSWRST;						//��ɼĴ�������
	//UCB2IE |= UCRXIE;							//ʹ���ж�
}
/*******************************************************************************
* ������	: SendByteToSPI2
* ����		: SPI3�����ֽں���
* �������  : chr
* ���ز���  : ��
*******************************************************************************/
void SendByteToSPI2(unsigned char chr)
{
	UCB2TXBUF = chr;        // Send the data sitting at the pointer DATA to the TX Buffer
	while(!(UCB2IFG & UCTXIFG));
}
/*******************************************************************************
* Function Name  : SendNByteToSPI2
* Description    :
* Input para     : data,len
* Output para    : None
*******************************************************************************/
void SendNByteToSPI2(unsigned char *data,unsigned char len)
{
	static unsigned char m = 0;
	for(m=0;m<len;m++)
	{
		UCB2TXBUF = data[m];
		while(!(UCB2IFG & UCTXIFG));
	}
}
/*******************************************************************************
* ������	: SPI2_ReadWriteByte ����SD��
* ����	: SPI��д����
* �������  : TxData
* ���ز���  : RXBUF1
*******************************************************************************/
unsigned char SPI2_ReadWriteByte(unsigned char TxData)
{
	unsigned char delayVar;

	while(!(UCB2IFG & UCTXIFG));
	UCB2TXBUF = TxData;           // Send the data sitting at the pointer DATA to the TX Buffer
	while(!(UCB2IFG & UCRXIFG));
	delayVar = UCB2RXBUF;

	return delayVar;
}
/*******************************************************************************
* ������	: SPI_B3_Init
* ����		: SPI_B3��ʼ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void Init_SPI3(void)
{
	P2SEL |= BIT2+BIT3+BIT4;						// Set P2.2-5 as SPI peripheral
	UCB3CTL1 |= UCSWRST;               				// Enable SW reset
	UCB3CTL0 |= UCMSB+UCMST+UCSYNC+UCCKPL;			//3��SPIģʽ����UCxSTE=1ʱ�ӻ�ʹ��
													//8λ����SPI���������״̬Ϊ�ߵ�ƽ����λ��ǰ
	UCB3CTL1 |= UCSSEL_2;                     		// ѡ��ο�ʱ��ΪSCMLK=16MHz
	UCB3BR0 = 2;									//2��Ƶ
	UCB3BR1 = 0;
	UCB3CTL1 &= ~UCSWRST;							//��ɼĴ�������
	UCB3IE |= UCRXIE;								//ʹ���ж�
}
/*******************************************************************************
* ������	: SendByteToSPI3
* ����		: SPI3�����ֽں���
* �������  : chr
* ���ز���  : ��
*******************************************************************************/
void SendByteToSPI3(unsigned char chr)
{
	UCB3TXBUF = chr;        // Send the data sitting at the pointer DATA to the TX Buffer
	while(!(UCB3IFG & UCTXIFG));
}
/*******************************************************************************
* Function Name  : SendNByteToSPI3
* Description    :
* Input para     : data,len
* Output para    : None
*******************************************************************************/
void SendNByteToSPI3(unsigned char *data,unsigned char len)
{
	static unsigned char m = 0;
	for(m=0;m<len;m++)
	{
		UCB3TXBUF = data[m];        // Send the data sitting at the pointer DATA to the TX Buffer
		while(!(UCB3IFG & UCTXIFG));
	}
}


//------USCI_B2�жϷ��������-------------------------------------------------+
//#pragma vector=USCI_B2_VECTOR
//__interrupt void USCI_B2_ISR(void)
//{
//  switch(__even_in_range(UCB2IV,4))
//  {
//  case 0:break;                             // Vector 0 - no interrupt
//  case 2:                                   // Vector 2 - RXIFG
//      while(!(UCB2IFG&UCTXIFG));            // USCI_B0 TX buffer ready?
//	  {
//	  }
//      break;
//  case 4:break;                             // Vector 4 - TXIFG
//  default: break;
//  }
//}
////------USCI_B3�жϷ��������-------------------------------------------------+
//#pragma vector=USCI_B3_VECTOR
//__interrupt void USCI_B3_ISR(void)
//{
//  switch(__even_in_range(UCB3IV,4))
//  {
//  case 0:break;                             // Vector 0 - no interrupt
//  case 2:                                   // Vector 2 - RXIFG
//      while(!(UCB3IFG&UCTXIFG));            // USCI_B0 TX buffer ready?
//	  {
//	  }
//      break;
//  case 4:break;                             // Vector 4 - TXIFG
//  default: break;
//  }
//}
