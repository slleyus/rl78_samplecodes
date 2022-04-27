/*
*******************************************************************************
** Pragma directive
*******************************************************************************
*/
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "stl_macrodriver.h"
#include "stl_adc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "stl_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function initializes the AD converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Create(void)
{
	ADCEN = 1U;	/* supply AD clock */
	ADM0 = _00_AD_ADM0_INITIALVALUE;	/* disable AD conversion and clear ADM0 register */
	ADMK = 1U;	/* disable INTAD interrupt */
	ADIF = 0U;	/* clear INTAD interrupt flag */
	/* Set INTAD low priority */
	ADPR1 = 1U;
	ADPR0 = 1U;
	/* Set ANI0 - ANI7 pin as analog input */
	PM2 |= 0xFFU;
	/* Set ANI16 pin */
	PMC0 |= 0x08U;
	PM0 |= 0x08U;
	/* Set ANI17 pin */
	PMC0 |= 0x04U;
	PM0 |= 0x04U;
	/* Set ANI18 pin */
	PMC14 |= 0x80U;
	PM14 |= 0x80U;
	/* Set ANI19 pin */
	PMC12 |= 0x01U;
	PM12 |= 0x01U;
	ADM0 = _08_AD_CONVERSION_CLOCK_32 | _00_AD_TIME_MODE_NORMAL_1 | _00_AD_OPERMODE_SELECT;
	ADM1 = _00_AD_TRIGGER_SOFTWARE | _00_AD_CONVMODE_CONSELECT;
	ADM2 = _00_AD_POSITIVE_VDD | _00_AD_NEGATIVE_VSS | _00_AD_AREA_MODE_1 | _00_AD_RESOLUTION_10BIT;
	ADUL = _FF_AD_ADUL_VALUE;
	ADLL = _00_AD_ADLL_VALUE;
	ADS = _00_AD_INPUT_CHANNEL_0;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function starts the AD converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Start(void)
{
	ADIF = 0U;	/* clear INTAD interrupt flag */
	ADMK = 0U;	/* enable INTAD interrupt */
	ADM0 = 0x81;
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops the AD converter.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Stop(void)
{
	ADCS = 0U;	/* disable AD conversion */
	ADMK = 1U;	/* disable INTAD interrupt */
	ADIF = 0U;	/* clear INTAD interrupt flag */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function enables comparator operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Set_OperationOn(void)
{
	ADCE = 1U;	/* enable AD comparator */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function stops comparator operation.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Set_OperationOff(void)
{
	ADCE = 0U;	/* disable AD comparator */
}
/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function returns the conversion result in the buffer.
**
**  Parameters:
**	buffer: the address where to write the conversion result
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void R_ADC_Get_Result(uint16_t *buffer)
{
	*buffer = (uint16_t)(ADCR >> 6U);
}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
