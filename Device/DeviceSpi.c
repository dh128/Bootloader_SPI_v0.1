
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
	UCB2CTL0 |= UCCKPH+UCMSB+UCMST+UCSYNC;            //+UCCKPL;  //3线SPI模式，当UCxSTE=1时从机使能
	                                                  //UCCKPH(SD CARD需要在上升沿读写数据且UCCKPL==0）
													  //8位数据SPI主机，不活动状态为高电平，高位在前
	UCB2CTL1 |= UCSSEL__SMCLK;                        // 选择参考时钟为SCMLK=16MHz
	UCB2BR0 = 6;								      //6分频
	UCB2BR1 = 0;
	UCB2CTL1 &= ~UCSWRST;						      //完成寄存器设置
	//UCB2IE |= UCRXIE;							      //使能中断

}

/*******************************************************************************
* 函数名	: SPIx_ReadWriteByte
* 描述	: SPI读写函数
* 输入参数  : TxData
* 返回参数  : RXBUF1
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
* 函数名	: SPI_B2_Init
* 描述:    SPI_B2初始化
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void Init_SPI2(void)
{
	P7SEL |= BIT2+BIT3+BIT4;				    // Set P7.2-4 as SPI peripheral
	P7DIR |= BIT2+BIT3+BIT5;

//	SD_CS_High();                               //选通SD卡

	UCB2CTL1 |= UCSWRST;               		    // Enable SW reset
	UCB2CTL0 |= UCCKPH+UCMSB+UCMST+UCSYNC;      //+UCCKPL;//3线SPI模式，当UCxSTE=1时从机使能
	                                            //UCCKPH(SD CARD需要在上升沿读写数据且UCCKPL==0）
											    //8位数据SPI主机，不活动状态为高电平，高位在前
	UCB2CTL1 |= UCSSEL__SMCLK;                  // 选择参考时钟为SCMLK=16MHz
	UCB2BR0 = 0;								//6分频
	UCB2BR1 = 0;
	UCB2CTL1 &= ~UCSWRST;						//完成寄存器设置
	//UCB2IE |= UCRXIE;							//使能中断
}
/*******************************************************************************
* 函数名	: SendByteToSPI2
* 描述		: SPI3发送字节函数
* 输入参数  : chr
* 返回参数  : 无
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
* 函数名	: SPI2_ReadWriteByte 用于SD卡
* 描述	: SPI读写函数
* 输入参数  : TxData
* 返回参数  : RXBUF1
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
* 函数名	: SPI_B3_Init
* 描述		: SPI_B3初始化
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void Init_SPI3(void)
{
	P2SEL |= BIT2+BIT3+BIT4;						// Set P2.2-5 as SPI peripheral
	UCB3CTL1 |= UCSWRST;               				// Enable SW reset
	UCB3CTL0 |= UCMSB+UCMST+UCSYNC+UCCKPL;			//3线SPI模式，当UCxSTE=1时从机使能
													//8位数据SPI主机，不活动状态为高电平，高位在前
	UCB3CTL1 |= UCSSEL_2;                     		// 选择参考时钟为SCMLK=16MHz
	UCB3BR0 = 2;									//2分频
	UCB3BR1 = 0;
	UCB3CTL1 &= ~UCSWRST;							//完成寄存器设置
	UCB3IE |= UCRXIE;								//使能中断
}
/*******************************************************************************
* 函数名	: SendByteToSPI3
* 描述		: SPI3发送字节函数
* 输入参数  : chr
* 返回参数  : 无
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


//------USCI_B2中断服务服务函数-------------------------------------------------+
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
////------USCI_B3中断服务服务函数-------------------------------------------------+
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
