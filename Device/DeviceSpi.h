#ifndef DEVICESPI_H
#define DEVICESPI_H

#define SD_CS_High()	P7OUT |= BIT5		//SD_cs put high
#define SD_CS_Low()		P7OUT &=~BIT5		//SD_cs put low Enable
#define CS_1 			P2OUT |= BIT2		//SPI falsh cs put high
#define CS_0 			P2OUT &=~BIT2		//SPI falsh cs put low


void SPIx_Init(void);
unsigned char SPIx_ReadWriteByte(unsigned char TxData);
void Init_SPI2(void);
void SendByteToSPI2(unsigned char chr);
void SendNByteToSPI2(unsigned char *data,unsigned char len);
unsigned char SPI2_ReadWriteByte(unsigned char TxData);
void Init_SPI3(void);
void SendByteToSPI3(unsigned char chr);
void SendNByteToSPI3(unsigned char *data,unsigned char len);

#define Spi_WriteByte   SPI2_ReadWriteByte
#define Spi_ReadByte()  SPI2_ReadWriteByte(0xff)

#endif




