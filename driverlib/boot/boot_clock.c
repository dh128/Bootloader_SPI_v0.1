#include "..\..\App\main.h"

/*******************************************************************************
* 函数名	: CloseWatchDog
* 描述	: 关闭看门口定时器
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void CloseWatchDog(void)
{
	WDTCTL = WDTPW + WDTHOLD;       //关闭看门狗
}

/*******************************************************************************
* 函数名	: InitClock
* 描述	: MCLK=SMCLK=16Mhz
*         ACLK=32.768kHz
* 输入参数  : 无
* 返回参数  : 无
*******************************************************************************/
void InitClock(void)
{
    // Use the REFO oscillator as the FLL reference, and also for ACLK
    UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
    UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);
    // Start the FLL, which will drive MCLK (not the crystal)
    Init_FLL(MCLK_FREQ/1000, MCLK_FREQ/32768);
}
