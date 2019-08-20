/****************SPI_Flash.c*************************************/

#include "..\App\main.h"



//оƬдʹ��
unsigned int PageNum = 0;
unsigned long ByteNum = 0;
unsigned long readAddr = 0;
void Write_Enable(void)
{
  CS_0;  
  delay_us(1);
  Spi_WriteByte(WRITE_ENABLE);  
  delay_us(1);
  CS_1;
}

//оƬ��ֹд��

void Write_Disable(void)

{
  CS_0;  
  delay_us(1);
  Spi_WriteByte(WRITE_DISABLE);
  delay_us(1);
  CS_1;
}

//��ȡоƬ״̬�Ĵ���

unsigned char Read_StatReg(void)
{
  unsigned char temp;
  CS_0;  
  delay_us(1);
  Spi_WriteByte(READ_STATUSREG);  
  temp = Spi_ReadByte();  
  delay_us(1);
  CS_1;  
  return temp;
}

//д״̬�Ĵ���

void Write_StatReg(unsigned char com)
{
  CS_0;  
  delay_us(1);
  Spi_WriteByte(WRITE_STATUSREG);  
  Spi_WriteByte(com);  
  delay_us(1);
  CS_1;
}

void Erase_Secotr(long address)
{
  unsigned char H,M,L;
  H = address>>16;  
  M = address>>8;  
  L = address&0xff;  
  Write_Enable(); //��ִ��дʹ�� 
  CS_0;  
  delay_us(1);
  Spi_WriteByte(CLEAR_SECTOR);
  Spi_WriteByte(H);  
  Spi_WriteByte(M);  
  Spi_WriteByte(L); 
  delay_us(1);
  CS_1;
}
void Erase_Block(long address)
{
  unsigned char H,M,L;
  H = address>>16;
  M = address>>8;
  L = address&0xff;
  Write_Enable(); //��ִ��дʹ��
  CS_0;
  delay_us(1);
  Spi_WriteByte(CLEAR_BLOCK);
  Spi_WriteByte(H);
  Spi_WriteByte(M);
  Spi_WriteByte(L);
  delay_us(1);
  CS_1;
}
void Erase_Chip(void)
{
  Write_Enable(); //��ִ��дʹ��
  CS_0;
  delay_us(1);
  Spi_WriteByte(CLEAR_SHIP);
  delay_us(1);
  CS_1;
}
//�������ַд��һ���ֽ�

void Write_Byte(long address,unsigned char byte)

{
  unsigned char H,M,L;
  H = address>>16;  
  M = address>>8; 
  L = address&0xff;  
  CS_0;  
  delay_us(1);
  Spi_WriteByte(WRITE_PAGE);  
  Spi_WriteByte(H);  
  Spi_WriteByte(M);  
  Spi_WriteByte(L);  
  Spi_WriteByte(byte);
  delay_us(1);
  CS_1;
}

//�������ַ��ʼд��һ�����ݰ�����󳤶Ȳ�����256���ֽڣ�

void Write_Date(long address,unsigned char Date_Buf[],unsigned char size)

{  
  unsigned char i;
  unsigned char H,M,L;
  H = address>>16;  
  M = address>>8;  
  L = address&0xff;  
  CS_0;  
  delay_us(1);
  Spi_WriteByte(WRITE_PAGE);  
  Spi_WriteByte(H);  
  Spi_WriteByte(M);  
  Spi_WriteByte(L);
  for(i=0;i<size;i++)  
  {  
    Spi_WriteByte(Date_Buf[i]);
  }  
  delay_us(1);
  CS_1;
}

//�������ַ����һ���ֽ�

unsigned char Read_Byte(long address)

{
  unsigned char temp;
  unsigned char H,M,L;
  H = address>>16; 
  M = address>>8;  
  L = address&0xff; 
  CS_0;  
  delay_us(1);
  Spi_WriteByte(READ_DATE);  
  Spi_WriteByte(H);  
  Spi_WriteByte(M);  
  Spi_WriteByte(L);  
  temp = Spi_ReadByte();  
  delay_us(1);
  CS_1;  
  return temp;
}

//�������ַ��ʼ��������

void Read_Data(long address,unsigned char Date_Buf[],unsigned char size)

{
  unsigned char i;
  unsigned char H,M,L;
  H = address>>16;  
  M = address>>8;  
  L = address&0xff;  
  CS_0;  
  delay_us(1);
  Spi_WriteByte(READ_DATE);  
  Spi_WriteByte(H);  
  Spi_WriteByte(M); 
  Spi_WriteByte(L); 
  for(i=0;i<size;i++) 
  {  
    Date_Buf[i] = Spi_ReadByte();  
  }  
  delay_us(1);
  CS_1;
}
