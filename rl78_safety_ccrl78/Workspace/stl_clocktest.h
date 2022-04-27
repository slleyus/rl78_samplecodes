;/*******************************************************************************/
;/* DISCLAIMER*/
;/* This software is supplied by Renesas Electronics Corporation and is only*/
;/* intended for use with Renesas products. No other uses are authorized. This*/
;/* software is owned by Renesas Electronics Corporation and is protected under*/
;/* all applicable laws, including copyright laws.*/
;/* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING*/
;/* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT*/
;/* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE*/
;/* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.*/
;/* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS*/
;/* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE*/
;/* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR*/
;/* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE*/
;/* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.*/
;/* Renesas reserves the right, without notice, to make changes to this software*/
;/* and to discontinue the availability of this software. By using this software,*/
;/* you agree to the additional terms and conditions found by accessing the*/
;/* following link:*/
;/* http://www.renesas.com/disclaimer*/
;/* Copyright (C) 2016 Renesas Electronics Corporation. All rights reserved.*/
;*********************************************************************************/
;/********************************************************************************/
;/** File Name : stl_clocktest.h*/
;/** Version : 2.00 */
;/** Device(s) : RL78/G14*/
;/** Tool-Chain : RL78 CC-RL V1.03.00*/
;/** OS : None*/
;/** H/W Platform : RL78 Target Board*/
;/** Description : RL78 IEC Self Test Library Clock Test modules Header File               */
;/** Operation : Defines user configuration for Clock Test modules*/
;/** Limitations : None*/
;/**********************************************************************************/
;/** History :*/
;/** 20.03.2011 V0.10 Pre Release*/
;/** 15.11.2013 V1.10 Fixed some comments */
;/** 07.11.2016 V2.00 CC-RL Tool-Chain
;/********************************************************************************/


;/********************************************************************************/
;/*Macro Definitions*/
;/********************************************************************************/
;/* Software clock test input port definition */ 
TESTPORT 				.EQU    P0.2                   
TEST_PORTMODE  			.EQU	PM0.2             

;/* Hardware clock test Capture interrupt flag definition */ 
CAPTURE_interrupt_FLAG  .EQU 	IF1L.5         
CAPTURE_interrupt_1  	.EQU 	IF1L.4         
CAPTURE_interrupt_2  	.EQU 	IF1L.6         

;/* Hardware clock test reference values */ 
hwMAXTIME  				.EQU 	0x03DF                      
hwMINTIME  				.EQU 	0x03C0                     

;/* Software clock test reference values */ 
swMAXTIME  				.EQU 	0x0060                     
swMINTIME  				.EQU 	0x0030                     
