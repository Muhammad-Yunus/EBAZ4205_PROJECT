/*
 * LCD_Driver.h
 *
 *  Created on: Aug 23, 2025
 *      Author: yunus
 */

#ifndef SRC_LCD_DRIVER_H_
#define SRC_LCD_DRIVER_H_

#include "GPIO.h"
#include "SPI.h"
#include <stdint.h>
#include "Delay.h"

#define	COLOR				uint16_t		//The variable type of the color (unsigned short)
#define	POINT				uint16_t		//The type of coordinate (unsigned short)
#define	LENGTH				uint16_t		//The type of coordinate (unsigned short)

/********************************************************************************
function:
		Define the full screen height length of the display
********************************************************************************/
#define LCD_1IN44
//#define LCD_1IN8
#if defined(LCD_1IN44)
	#define LCD_WIDTH  128  //LCD width
	#define LCD_HEIGHT  128 //LCD height
	#define LCD_X	2
	#define LCD_Y	1
#elif defined(LCD_1IN8)
	#define LCD_WIDTH  160  //LCD width
	#define LCD_HEIGHT  128 //LCD height
	#define LCD_X	 1
	#define LCD_Y	 2
#endif

#define LCD_X_MAXPIXEL  128  	//LCD width maximum memory
#define LCD_Y_MAXPIXEL  128 	//LCD height maximum memory


/********************************************************************************
function:
			scanning method
********************************************************************************/
typedef enum{
	L2R_U2D  = 0,	//The display interface is displayed , left to right, up to down
	L2R_D2U  ,
	R2L_U2D  ,
	R2L_D2U  ,

	U2D_L2R  ,
	U2D_R2L  ,
	D2U_L2R  ,
	D2U_R2L  ,
}LCD_SCAN_DIR;
#define SCAN_DIR_DFT  U2D_R2L  //Default scan direction = L2R_U2D

/********************************************************************************
function:
	Defines the total number of rows in the display area
********************************************************************************/

typedef struct{
	LENGTH LCD_Dis_Column;	//COLUMN
	LENGTH LCD_Dis_Page;	//PAGE
	LCD_SCAN_DIR LCD_Scan_Dir;
	POINT LCD_X_Adjust;		//LCD x actual display position calibration
	POINT LCD_Y_Adjust;		//LCD y actual display position calibration
}LCD_DIS;


void LCD_Reset(void);
void LCD_WriteReg(uint8_t Reg);
void LCD_WriteData_8Bit(uint8_t Data);
void LCD_WriteData_NLen16Bit(uint16_t Data,uint32_t DataLen);
void LCD_InitReg(void);
void LCD_SetGramScanWay(LCD_SCAN_DIR Scan_dir);
#endif /* SRC_LCD_DRIVER_H_ */
