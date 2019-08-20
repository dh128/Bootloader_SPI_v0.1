#include "..\..\App\main.h"

/*******************************************************************************
* ������	: CloseWatchDog
* ����	: �رտ��ſڶ�ʱ��
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void CloseWatchDog(void)
{
	WDTCTL = WDTPW + WDTHOLD;       //�رտ��Ź�
}

/*******************************************************************************
* ������	: InitClock
* ����	: MCLK=SMCLK=16Mhz
*         ACLK=32.768kHz
* �������  : ��
* ���ز���  : ��
*******************************************************************************/
void InitClock(void)
{
    // Use the REFO oscillator as the FLL reference, and also for ACLK
    UCSCTL3 = (UCSCTL3 & ~(SELREF_7)) | (SELREF__REFOCLK);
    UCSCTL4 = (UCSCTL4 & ~(SELA_7)) | (SELA__REFOCLK);
    // Start the FLL, which will drive MCLK (not the crystal)
    Init_FLL(MCLK_FREQ/1000, MCLK_FREQ/32768);
}
