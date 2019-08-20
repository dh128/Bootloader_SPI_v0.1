/*******************************************************************************
*    File Name:  DeviceUart.h
*    Revision:  1.0
*    Description:  串口程序头文件
* *****************************************************************************/
#ifndef MAIN_H
#define MAIN_H

#include <msp430.h>

#include <intrinsics.h>
#include <string.h>
#include <math.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>

#include "..\driverlib\MSP430F5xx_6xx\flashctl.h"


#include "..\driverlib\boot\boot_clock.h"
#include "..\driverlib\boot\boot_flash.h"
#include "..\driverlib\boot\boot_gpio.h"
#include "..\driverlib\boot\boot_uart.h"
#include "..\driverlib\common\hal_device.h"
#include "..\driverlib\common\hal_pmm.h"
#include "..\driverlib\common\hal_tlv.h"
#include "..\driverlib\common\hal_types.h"
#include "..\driverlib\common\hal_ucs.h"


#include "..\Device\DeviceSpi.h"
#include "..\Device\DeviceSPIFlash.h"


#define Power_3V_ON		      P4OUT |= BIT0	    //打开外设3V3电源
#define Power_3V_OFF		  P4OUT &=~BIT0	    //关闭外设3V3电源
#endif
