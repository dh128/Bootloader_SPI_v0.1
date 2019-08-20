/****************SPI_Flash.c*************************************/

#include "..\App\main.h"



//芯片写使能
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

//芯片禁止写入

void Write_Disable(void)

{
  CS_0;  
  delay_us(1);
  Spi_WriteByte(WRITE_DISABLE);
  delay_us(1);
  CS_1;
}

//读取芯片状态寄存器

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

//写状态寄存器

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
  Write_Enable(); //先执行写使能 
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
  Write_Enable(); //先执行写使能
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
  Write_Enable(); //先执行写使能
  CS_0;
  delay_us(1);
  Spi_WriteByte(CLEAR_SHIP);
  delay_us(1);
  CS_1;
}
//在任意地址写入一个字节

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

//在任意地址开始写入一个数据包（最大长度不超过256个字节）

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

//在任意地址读出一个字节

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

//从任意地址开始读出数据

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
