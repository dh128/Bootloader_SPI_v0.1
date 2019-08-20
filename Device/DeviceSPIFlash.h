/*******************************************

程序说明：SPI_Flash读写程序

硬件连接：W25X16存储芯片

引脚说明：CS-P2.2CLK-P3.3; DIO-P3.1; DO-P3.2

Author  ：Dinghui


W25X16  16M-bit/2M Bytes	== 8192pages
paogram:
page paogram

擦出操作：
block		4kB
sector		64kB
chip		2M

Erase:
page	256bytes
sector	16pages
block 	256pages
entire chip
page0 - 1599		//400KB 用于存放Flash
page4 -	403			//存储code	403-4+1=400pages=100k



********************************************/
#ifndef DEVICESPIFLASH_H
#define DEVICESPIFLASH_H




//////W25X16 命令指令表定义
#define WRITE_ENABLE      0X06        //写使能，设置状态寄存器
#define WRITE_DISABLE     0X04        //写禁止
#define READ_STATUSREG    0X05        //读状态寄存器
#define WRITE_STATUSREG   0X01        //写状态寄存器
#define READ_DATE         0X03        //读取存储器数据
#define READ_FAST         0X0B        //快速读取存储器数据
#define READ_DOUBLEFAST   0X3B        //快速双端口输出方式读取存储器数据
#define WRITE_PAGE        0X02        //页面编程--写数据
#define CLEAR_BLOCK       0XD8        //块擦除	64Kb
#define CLEAR_SECTOR      0X20        //扇区擦除	4kb
#define CLEAR_SHIP        0XC7        //片擦除	all
#define POWER_OFF         0XB9        //掉电模式
#define POWER_ON          0XAB        //退出掉电模式、设备ID信息
#define SHIP_ID           0X90        //读取制造厂商ID信息和设备ID信息
#define JEDEC_ID          0X9F        //JEDEC的ID信息

///////函数声明
extern void Write_Enable(void);
extern void Write_Disable(void);
extern unsigned char Read_StatReg(void);
extern void Write_StatReg(unsigned char com);
extern void Erase_Secotr(long address);
void Erase_Block(long address);
void Erase_Chip(void);
extern unsigned char Read_Byte(long address);
extern void Read_Data(long address,unsigned char Date_Buf[],unsigned char size);
extern void Write_Byte(long address,unsigned char byte);
extern void Write_Date(long address,unsigned char Date_Buf[],unsigned char size);


extern unsigned int PageNum;
extern unsigned long ByteNum;
extern unsigned long readAddr;

#endif
