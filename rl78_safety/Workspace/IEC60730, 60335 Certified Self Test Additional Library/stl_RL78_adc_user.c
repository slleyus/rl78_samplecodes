/*
*******************************************************************************
** Pragma directive
*******************************************************************************
*/
#pragma interrupt INTAD R_ADC_Interrupt
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
**	This function is INTAD interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
__interrupt void R_ADC_Interrupt(void)
{
	/* Start user code. Do not edit comment generated here */
	uint16_t buffer;
	buffer = (uint16_t)(ADCR >> 6U);
	/* End user code. Do not edit comment generated here */

}

/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
