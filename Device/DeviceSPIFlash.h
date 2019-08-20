/*******************************************

����˵����SPI_Flash��д����

Ӳ�����ӣ�W25X16�洢оƬ

����˵����CS-P2.2CLK-P3.3; DIO-P3.1; DO-P3.2

Author  ��Dinghui


W25X16  16M-bit/2M Bytes	== 8192pages
paogram:
page paogram

����������
block		4kB
sector		64kB
chip		2M

Erase:
page	256bytes
sector	16pages
block 	256pages
entire chip
page0 - 1599		//400KB ���ڴ��Flash
page4 -	403			//�洢code	403-4+1=400pages=100k



********************************************/
#ifndef DEVICESPIFLASH_H
#define DEVICESPIFLASH_H




//////W25X16 ����ָ�����
#define WRITE_ENABLE      0X06        //дʹ�ܣ�����״̬�Ĵ���
#define WRITE_DISABLE     0X04        //д��ֹ
#define READ_STATUSREG    0X05        //��״̬�Ĵ���
#define WRITE_STATUSREG   0X01        //д״̬�Ĵ���
#define READ_DATE         0X03        //��ȡ�洢������
#define READ_FAST         0X0B        //���ٶ�ȡ�洢������
#define READ_DOUBLEFAST   0X3B        //����˫�˿������ʽ��ȡ�洢������
#define WRITE_PAGE        0X02        //ҳ����--д����
#define CLEAR_BLOCK       0XD8        //�����	64Kb
#define CLEAR_SECTOR      0X20        //��������	4kb
#define CLEAR_SHIP        0XC7        //Ƭ����	all
#define POWER_OFF         0XB9        //����ģʽ
#define POWER_ON          0XAB        //�˳�����ģʽ���豸ID��Ϣ
#define SHIP_ID           0X90        //��ȡ���쳧��ID��Ϣ���豸ID��Ϣ
#define JEDEC_ID          0X9F        //JEDEC��ID��Ϣ

///////��������
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
