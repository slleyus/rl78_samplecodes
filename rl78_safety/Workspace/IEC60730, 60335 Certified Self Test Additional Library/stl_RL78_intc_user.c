
/*
*******************************************************************************
** Pragma directive
*******************************************************************************
*/
#pragma interrupt INTP0 R_INTC0_Interrupt
/* Start user code for pragma. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
/*
*******************************************************************************
** Include files
*******************************************************************************
*/
#include "stl_macrodriver.h"
#include "stl_intc.h"
/* Start user code for include. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#include "stl_userdefine.h"

/*
*******************************************************************************
**  Global define
*******************************************************************************
*/
/* Start user code for global. Do not edit comment generated here */
int SW_CNT;
/* End user code. Do not edit comment generated here */

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function is INTP0 interrupt service routine.
**
**  Parameters:
**	None
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
__interrupt void R_INTC0_Interrupt(void)
{
	/* Start user code. Do not edit comment generated here */
	P6.2=~P6.2;
	SW_CNT++;
	/* End user code. Do not edit comment generated here */
}




/* Start user code for adding. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
