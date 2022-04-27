
#include <r_cg_macrodriver.h>
#include "flash_interface.h"

__near fsl_write_t fsl_write_str;

/********************************************************************************
* Function Name : init_fcl
* Description   : This function initializes the FSL driver.
* Parameters    : None
* Return value  : None
********************************************************************************/
uint8_t init_fcl(void)
{
    fsl_u08             fsl_err;
    fsl_descriptor_t    fsl_descr;
    fsl_fsw_t           fsl_window;


    /* initialize self-programming environment */

    fsl_descr.fsl_flash_voltage_u08 = 0x00;
    fsl_descr.fsl_frequency_u08 = 0x08;
    fsl_descr.fsl_auto_status_check_u08 = 0x01;

    fsl_err = FSL_Init((__far fsl_descriptor_t*)&fsl_descr);
    if(fsl_err != FSL_OK)
    {
        return fsl_err;
    }

    FSL_Open();
    FSL_PrepareFunctions();
    FSL_PrepareExtFunctions();

    fsl_window.fsl_start_block_u16 = 0;
    fsl_window.fsl_end_block_u16 = BLOCK_APP_END;
    FSL_SetFlashShieldWindow(&fsl_window);

    return FSL_OK;
}


uint8_t flash_erase_app_area(void)
{
	uint16_t    i;
	uint8_t     fsl_status;

	for (i = BLOCK_BOOT1_BEGIN; i <= BLOCK_APP_END; i++)
	{
		fsl_status = FSL_Erase(i);
		if (fsl_status != FSL_OK)
		{
			return fsl_status;
		}
	}

	return fsl_status;
}

uint8_t flash_write_packet_data(uint32_t addr, uint8_t data[], uint16_t size)
{
	/* Setup data handler for write */
    fsl_write_str.fsl_data_buffer_p_u08 = (__near fsl_u08 *) data;
    fsl_write_str.fsl_word_count_u08 = (uint8_t)(size/4);
    fsl_write_str.fsl_destination_address_u32 = addr;

	return FSL_Write((__near fsl_write_t*) &fsl_write_str);
}

void flash_read_packet_data(uint32_t addr, uint8_t data[], uint16_t size)
{
	 uint8_t __far *ptr = (uint8_t __far *)addr;
	 uint8_t i;

	 for(i = 0; i < size; i++)
	 {
		 data[i] = ptr[i];
	 }
}

uint8_t flash_boot_swap_reset(void)
{
	fsl_u08 fsl_status;

	/* Invert boot flag to prepare for boot swap */
	fsl_status = FSL_InvertBootFlag();

	if( fsl_status != 0x00 )
	{
		return fsl_status;
	}

	/* Generate a reset */
	FSL_ForceReset();

	/* Should never occur, this silences compiler */
	return fsl_status;
}


























