/*******************************************************************************
* DISCLAIMER
* This software is supplied by Renesas Electronics Corporation and is only 
* intended for use with Renesas products. No other uses are authorized. This 
* software is owned by Renesas Electronics Corporation and is protected under
* all applicable laws, including copyright laws.
* THIS SOFTWARE IS PROVIDED "AS IS" AND RENESAS MAKES NO WARRANTIES REGARDING
* THIS SOFTWARE, WHETHER EXPRESS, IMPLIED OR STATUTORY, INCLUDING BUT NOT
* LIMITED TO WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE 
* AND NON-INFRINGEMENT. ALL SUCH WARRANTIES ARE EXPRESSLY DISCLAIMED.
* TO THE MAXIMUM EXTENT PERMITTED NOT PROHIBITED BY LAW, NEITHER RENESAS 
* ELECTRONICS CORPORATION NOR ANY OF ITS AFFILIATED COMPANIES SHALL BE LIABLE 
* FOR ANY DIRECT, INDIRECT, SPECIAL, INCIDENTAL OR CONSEQUENTIAL DAMAGES FOR
* ANY REASON RELATED TO THIS SOFTWARE, EVEN IF RENESAS OR ITS AFFILIATES HAVE
* BEEN ADVISED OF THE POSSIBILITY OF SUCH DAMAGES.
* Renesas reserves the right, without notice, to make changes to this software
* and to discontinue the availability of this software. By using this software,
* you agree to the additional terms and conditions found by accessing the 
* following link:
* http://www.renesas.com/disclaimer *
* Copyright (C) 2011 Renesas Electronics Corporation. All rights reserved.    
*******************************************************************************/
/*******************************************************************************
* File Name	   : r_cs_template.c
* Version      : 1.0 <- Optional as long as history is shown below
* Description  : This module solves all the world's problems
******************************************************************************/
/*****************************************************************************
* History : DD.MM.YYYY Version  Description
*         : 15.01.2007 1.00     First Release
******************************************************************************/

/******************************************************************************
Includes   <System Includes> , "Project Includes"
******************************************************************************/
#include "C99Types.h"
#include "fdl_types.h"
#include "fdl.h"
#include "fdl_descriptor.h"
/******************************************************************************
Macro definitions
******************************************************************************/

/******************************************************************************
Typedef definitions
******************************************************************************/

/******************************************************************************
Exported global variables (to be accessed by other files)
******************************************************************************/
 
/******************************************************************************
Private global variables and functions
******************************************************************************/
/* Variables and functions for data flash access  */
fal_status_t my_status;
__near fal_request_t my_fal_WCMD_request, my_fal_BCMD_request;
/* Variables for data flash read and write operations  */

fal_u32 ReadDataFlash(fal_u16 index);
void WriteDataFlash(fal_u32 data, fal_u16 index);
void EraseDataFlash(fal_u16 index);

/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function writes data to the Data Flash.
**
**  Parameters:
**	data: Data to write
**      index: Location of data to erase
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void WriteDataFlash(fal_u32 data, fal_u16 index)
{
   if(my_status == FAL_OK)
   {
     my_status = FAL_Init(&fal_descriptor_str);
   }
   FAL_Open();

   my_fal_WCMD_request.data_u32 = data;
   my_fal_WCMD_request.index_u16 = index;
   my_fal_WCMD_request.command_enu = FAL_CMD_WRITE_WORD;
   
   /* Command Initiation */
   do {
   FAL_Execute(&my_fal_WCMD_request);
   FAL_Handler();                     // Proceed background process
   } while (my_fal_WCMD_request.status_enu == FAL_ERR_REJECTED);
   
   /* Command Execution */
   do {
   FAL_Handler();
   } while (my_fal_WCMD_request.status_enu == FAL_BUSY);
       
   FAL_Close();
}


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function reads data from the Data Flash.
**
**  Parameters:
**	index: Location of data to read
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
fal_u32 ReadDataFlash(fal_u16 index)
{
  fal_u32 my_data_u32;

  if(my_status == FAL_OK)
  {
    my_status = FAL_Init(&fal_descriptor_str);
  }
  FAL_Open();
   
  do {
  my_fal_WCMD_request.index_u16 = index;
  my_fal_WCMD_request.command_enu = FAL_CMD_READ_WORD;
  FAL_Execute(&my_fal_WCMD_request);
  FAL_Handler();                            // Enforce eventually blocking command
  } while((my_fal_WCMD_request.status_enu==FAL_ERR_REJECTED));
   
  /* Read-request accepted -> Read the data directly */
  if (my_fal_WCMD_request.status_enu==FAL_OK) {
    my_data_u32 = my_fal_WCMD_request.data_u32;
  }
  
  FAL_Close();
   
  return (my_data_u32);
}


/*
**-----------------------------------------------------------------------------
**
**  Abstract:
**	This function erases data from the Data Flash.
**
**  Parameters:
**	index: Location of data to read
**
**  Returns:
**	None
**
**-----------------------------------------------------------------------------
*/
void EraseDataFlash(fal_u16 index)
{
  if(my_status == FAL_OK)
  {
    my_status = FAL_Init(&fal_descriptor_str);
  }
  FAL_Open();
  
  my_fal_BCMD_request.index_u16 = index;
  my_fal_BCMD_request.command_enu = FAL_CMD_ERASE_BLOCK;
  FAL_Execute(&my_fal_BCMD_request);
  while(my_fal_BCMD_request.status_enu == FAL_BUSY) FAL_Handler();
  
  FAL_Close();
}
