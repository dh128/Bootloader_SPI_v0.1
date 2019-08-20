/*
 * ======== CRC_Modbus.h ========
 */
#ifndef BOOT_FLASH_H
#define BOOT_FLASH_H

#define   infor_ChargeAddr       0x1900
#define   infor_BootAddr         0x1800

//inforFLASH存储分布
/****************************************************************************************
 *   DevID_H DevID_L| DevSN_H DevSN_L|  PD_H   PD_M   PD_L  |终端 类型 | 通讯方式  |Deveui_H Deveui_L|
 *   0x1900  0x1901 | 0x1902  0x1903 | 0x1904 0x1905 0x1906 | 0x1907 |  0x1908 | 0x1909   0x190A |
 *
 *
 */
uint8_t infoFLASH_read(uint8_t index,uint32_t add);
void infoFLASH_write(uint8_t *data_ptr,uint8_t *flashAddr,uint16_t count);
void FlashRsvWrite(uint8_t *data_ptr,uint8_t index,uint32_t infostartaddr,uint8_t num);

uint8_t ResetVectorValid(void);

#endif
