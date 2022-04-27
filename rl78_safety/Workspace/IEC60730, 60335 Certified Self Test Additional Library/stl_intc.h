
#ifndef _MDINTC_
#define _MDINTC_
/*
*******************************************************************************
**  Register bit define
*******************************************************************************
*/

/*
	External Interrupt Rising Edge Enable Register 0 (EGP0)
*/
/* INTPn pin valid edge selection (EGPn) */
#define _01_INTP0_EDGE_RISING_SEL		0x01U	/* rising edge selected for INTP0 pin */
#define _00_INTP0_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP0 pin */
#define _02_INTP1_EDGE_RISING_SEL		0x02U	/* rising edge selected for INTP1 pin */
#define _00_INTP1_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP1 pin*/
#define _04_INTP2_EDGE_RISING_SEL		0x04U	/* rising edge selected for INTP2 pin */
#define _00_INTP2_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP2 pin */
#define _08_INTP3_EDGE_RISING_SEL		0x08U	/* rising edge selected for INTP3 pin */
#define _00_INTP3_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP3 pin */
#define _10_INTP4_EDGE_RISING_SEL		0x10U	/* rising edge selected for INTP4 pin */
#define _00_INTP4_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP4 pin */
#define _20_INTP5_EDGE_RISING_SEL		0x20U	/* rising edge selected for INTP5 pin */
#define _00_INTP5_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP5 pin */
#define _40_INTP6_EDGE_RISING_SEL		0x40U	/* rising edge selected for INTP6 pin */
#define _00_INTP6_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP6 pin */
#define _80_INTP7_EDGE_RISING_SEL		0x80U	/* rising edge selected for INTP7 pin */
#define _00_INTP7_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP7 pin */

/*
	External Interrupt Falling Edge Enable Register 0 (EGN0)
*/
/* INTPn pin valid edge selection (EGNn) */
#define _01_INTP0_EDGE_FALLING_SEL		0x01U	/* falling edge selected for INTP0 pin */
#define _00_INTP0_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP0 pin */
#define _02_INTP1_EDGE_FALLING_SEL		0x02U	/* falling edge selected for INTP1 pin */
#define _00_INTP1_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP1 pin */
#define _04_INTP2_EDGE_FALLING_SEL		0x04U	/* falling edge selected for INTP2 pin */
#define _00_INTP2_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP2 pin */
#define _08_INTP3_EDGE_FALLING_SEL		0x08U	/* falling edge selected for INTP3 pin */
#define _00_INTP3_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP3 pin */
#define _10_INTP4_EDGE_FALLING_SEL		0x10U	/* falling edge selected for INTP4 pin */
#define _00_INTP4_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP4 pin */
#define _20_INTP5_EDGE_FALLING_SEL		0x20U	/* falling edge selected for INTP5 pin */
#define _00_INTP5_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP5 pin */
#define _40_INTP6_EDGE_FALLING_SEL		0x40U	/* falling edge selected for INTP6 pin */
#define _00_INTP6_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP6 pin */
#define _80_INTP7_EDGE_FALLING_SEL		0x80U	/* falling edge selected for INTP7 pin */
#define _00_INTP7_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP7 pin */

/*
	External Interrupt Rising Edge Enable Register 1 (EGP1)
*/
/* INTPn pin valid edge selection (EGPn) */
#define _01_INTP8_EDGE_RISING_SEL		0x01U	/* rising edge selected for INTP8 pin */
#define _00_INTP8_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP8 pin */
#define _02_INTP9_EDGE_RISING_SEL		0x02U	/* rising edge selected for INTP9 pin */
#define _00_INTP9_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP9 pin*/
#define _04_INTP10_EDGE_RISING_SEL		0x04U	/* rising edge selected for INTP10 pin */
#define _00_INTP10_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP10 pin */
#define _08_INTP11_EDGE_RISING_SEL		0x08U	/* rising edge selected for INTP11 pin */
#define _00_INTP11_EDGE_RISING_UNSEL		0x00U	/* rising edge not selected for INTP11 pin*/

/*
	External Interrupt Falling Edge Enable Register 1 (EGN1)
*/
/* INTPn pin valid edge selection (EGNn) */
#define _01_INTP8_EDGE_FALLING_SEL		0x01U	/* falling edge selected for INTP8 pin */
#define _00_INTP8_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP8 pin */
#define _02_INTP9_EDGE_FALLING_SEL		0x02U	/* falling edge selected for INTP9 pin */
#define _00_INTP9_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP9 pin */
#define _04_INTP10_EDGE_FALLING_SEL		0x04U	/* falling edge selected for INTP10 pin */
#define _00_INTP10_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP10 pin */
#define _08_INTP11_EDGE_FALLING_SEL		0x08U	/* falling edge selected for INTP11 pin */
#define _00_INTP11_EDGE_FALLING_UNSEL		0x00U	/* falling edge not selected for INTP11 pin */

/*
	Key Return Mode Register (KRM)
*/
/* Key interrupt mode control (KRM0) */
#define _00_KR0_SIGNAL_DETECT_OFF		0x00U	/* not detect KR0 signal */
#define _01_KR0_SIGNAL_DETECT_ON		0x01U	/* detect KR0 signal */
/* Key interrupt mode control (KRM1) */
#define _00_KR1_SIGNAL_DETECT_OFF		0x00U	/* not detect KR1 signal */
#define _02_KR1_SIGNAL_DETECT_ON		0x02U	/* detect KR1 signal */        
/* Key interrupt mode control (KRM2) */
#define _00_KR2_SIGNAL_DETECT_OFF		0x00U	/* not detect KR2 signal */
#define _04_KR2_SIGNAL_DETECT_ON		0x04U	/* detect KR2 signal */        
/* Key interrupt mode control (KRM3) */
#define _00_KR3_SIGNAL_DETECT_OFF		0x00U	/* not detect KR3 signal */
#define _08_KR3_SIGNAL_DETECT_ON		0x08U	/* detect KR3 signal */        
/* Key interrupt mode control (KRM4) */
#define _00_KR4_SIGNAL_DETECT_OFF		0x00U	/* not detect KR4 signal */
#define _10_KR4_SIGNAL_DETECT_ON		0x10U	/* detect KR4 signal */        
/* Key interrupt mode control (KRM5) */
#define _00_KR5_SIGNAL_DETECT_OFF		0x00U	/* not detect KR5 signal */
#define _20_KR5_SIGNAL_DETECT_ON		0x20U	/* detect KR5 signal */        
/* Key interrupt mode control (KRM6) */
#define _00_KR6_SIGNAL_DETECT_OFF		0x00U	/* not detect KR6 signal */
#define _40_KR6_SIGNAL_DETECT_ON		0x40U	/* detect KR6 signal */        
/* Key interrupt mode control (KRM7) */
#define _00_KR7_SIGNAL_DETECT_OFF		0x00U	/* not detect KR7 signal */
#define _80_KR7_SIGNAL_DETECT_ON		0x80U	/* detect KR7 signal */        
/*
*******************************************************************************
**  Macro define
*******************************************************************************
*/
#define KEY_WAITTIME				8U	/* Wait 250 ns */ 
/*
*******************************************************************************
**  Function define
*******************************************************************************
*/
void R_INTC_Create(void);
void R_INTC0_Start(void);
void R_INTC0_Stop(void);
__interrupt void R_INTC0_Interrupt(void);

/* Start user code for function. Do not edit comment generated here */
/* End user code. Do not edit comment generated here */
#endif
