#include "..\..\App\main.h"


/*******************************************************************************
* Function Name  : infoFLASH_read
* Description    :
* Input para     : index add
* Output para    : value
*******************************************************************************/
uint8_t infoFLASH_read(uint8_t index,uint32_t add)
{
	uint32_t *Flash_ptrD;
	Flash_ptrD = (uint32_t *)add;
	uint8_t *flash_ptr =  ((uint8_t *)Flash_ptrD) + index;

	return *flash_ptr;
}

/*******************************************************************************
* Function Name  : infoFLASH_write
* Description    :
* Input para     : data_ptr flashAddr count
* Output para    : value
*******************************************************************************/
void infoFLASH_write(uint8_t *data_ptr,uint8_t *flashAddr,uint16_t count)
{
	FlashCtl_eraseSegment(flashAddr);

	if(FlashCtl_performEraseCheck(flashAddr,count) == 0x01)
	{
		FlashCtl_write8(data_ptr,flashAddr,count);
	}
}

void FlashRsvWrite(uint8_t *data_ptr,uint8_t index,uint32_t infostartaddr,uint8_t num)
{
	static uint8_t m;
	uint8_t cacheBuf[128];

	for(m=0;m<128;m++)
	{
		cacheBuf[m] = infoFLASH_read(m,infostartaddr);
	}

	for(m=0;m<index;m++)
		cacheBuf[m+num] = *data_ptr++;

	infoFLASH_write(cacheBuf,(uint8_t *)infostartaddr,128);
}

/*******************************************************************************
* Function Name  : ResetVectorValid
* Description    : 检查infor地址的内容是否为0xFF,如果是0xFF,则执行应用程序
* Input para     : None
* Output para    : 0 or 1
*******************************************************************************/
uint8_t ResetVectorValid(void)
{
	uint8_t Check_ID = 0;

	Check_ID = FlashCtl_performUpdateCheck((uint8_t *)infor_BootAddr);
	if(Check_ID == Enble_Update)
	{
		return Disable_App;           //执行升级程序
	}

	return Enble_App;                 //执行应用程序
}
