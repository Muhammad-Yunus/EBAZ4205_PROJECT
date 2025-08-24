/***********************************************************************************************************************
* | file      	:	LCD_Driver.c
* |	version		:	V1.0
* | date		:	2017-10-16
* | function	:	On the ST7735S chip driver and clear screen, drawing lines, drawing, writing
					and other functions to achieve
***********************************************************************************************************************/


#include "LCD_Driver.h"

#include <stdlib.h>		//itoa()
#include <stdio.h>

XSpi  SpiInstance;	 /* The instance of the SPI device */
XGpio gpio0;

LCD_DIS sLCD_DIS;

/***********************************************************************************************************************
			------------------------------------------------------------------------
			|\\\																///|
			|\\\						Drive layer								///|
			------------------------------------------------------------------------
***********************************************************************************************************************/
/*******************************************************************************
function:
			Hardware reset
*******************************************************************************/
void LCD_Reset(void)
{
    Gpio_High(&gpio0, GPIO_RESET);
    delay_ms(100);
    Gpio_Low(&gpio0, GPIO_RESET);
    delay_ms(100);
    Gpio_High(&gpio0, GPIO_RESET);
    delay_ms(100);
}

/*******************************************************************************
function:
		Write register address and data
*******************************************************************************/
void LCD_WriteReg(uint8_t Reg)
{
	Gpio_Low(&gpio0, GPIO_DC);
    SPI_Write(&SpiInstance,Reg);
}

void LCD_WriteData_8Bit(uint8_t Data)
{
	Gpio_High(&gpio0, GPIO_DC);
    SPI_Write(&SpiInstance,Data);
}


void LCD_WriteData_NLen16Bit(uint16_t Data,uint32_t DataLen)
{
    uint32_t i;
    Gpio_High(&gpio0, GPIO_DC);
    for(i = 0; i < DataLen; i++) {
        SPI_Write(&SpiInstance, (uint8_t)(Data >> 8) );
        SPI_Write(&SpiInstance, (uint8_t)(Data & 0XFF) );
    }
}

/*******************************************************************************
function:
		Common register initialization
*******************************************************************************/
void LCD_InitReg(void)
{
    //ST7735R Frame Rate
    LCD_WriteReg(0xB1);
    LCD_WriteData_8Bit(0x01);
    LCD_WriteData_8Bit(0x2C);
    LCD_WriteData_8Bit(0x2D);

    LCD_WriteReg(0xB2);
    LCD_WriteData_8Bit(0x01);
    LCD_WriteData_8Bit(0x2C);
    LCD_WriteData_8Bit(0x2D);

    LCD_WriteReg(0xB3);
    LCD_WriteData_8Bit(0x01);
    LCD_WriteData_8Bit(0x2C);
    LCD_WriteData_8Bit(0x2D);
    LCD_WriteData_8Bit(0x01);
    LCD_WriteData_8Bit(0x2C);
    LCD_WriteData_8Bit(0x2D);

    LCD_WriteReg(0xB4); //Column inversion
    LCD_WriteData_8Bit(0x07);

    //ST7735R Power Sequence
    LCD_WriteReg(0xC0);
    LCD_WriteData_8Bit(0xA2);
    LCD_WriteData_8Bit(0x02);
    LCD_WriteData_8Bit(0x84);
    LCD_WriteReg(0xC1);
    LCD_WriteData_8Bit(0xC5);

    LCD_WriteReg(0xC2);
    LCD_WriteData_8Bit(0x0A);
    LCD_WriteData_8Bit(0x00);

    LCD_WriteReg(0xC3);
    LCD_WriteData_8Bit(0x8A);
    LCD_WriteData_8Bit(0x2A);
    LCD_WriteReg(0xC4);
    LCD_WriteData_8Bit(0x8A);
    LCD_WriteData_8Bit(0xEE);

    LCD_WriteReg(0xC5); //VCOM
    LCD_WriteData_8Bit(0x0E);

    //ST7735R Gamma Sequence
    LCD_WriteReg(0xe0);
    LCD_WriteData_8Bit(0x0f);
    LCD_WriteData_8Bit(0x1a);
    LCD_WriteData_8Bit(0x0f);
    LCD_WriteData_8Bit(0x18);
    LCD_WriteData_8Bit(0x2f);
    LCD_WriteData_8Bit(0x28);
    LCD_WriteData_8Bit(0x20);
    LCD_WriteData_8Bit(0x22);
    LCD_WriteData_8Bit(0x1f);
    LCD_WriteData_8Bit(0x1b);
    LCD_WriteData_8Bit(0x23);
    LCD_WriteData_8Bit(0x37);
    LCD_WriteData_8Bit(0x00);
    LCD_WriteData_8Bit(0x07);
    LCD_WriteData_8Bit(0x02);
    LCD_WriteData_8Bit(0x10);

    LCD_WriteReg(0xe1);
    LCD_WriteData_8Bit(0x0f);
    LCD_WriteData_8Bit(0x1b);
    LCD_WriteData_8Bit(0x0f);
    LCD_WriteData_8Bit(0x17);
    LCD_WriteData_8Bit(0x33);
    LCD_WriteData_8Bit(0x2c);
    LCD_WriteData_8Bit(0x29);
    LCD_WriteData_8Bit(0x2e);
    LCD_WriteData_8Bit(0x30);
    LCD_WriteData_8Bit(0x30);
    LCD_WriteData_8Bit(0x39);
    LCD_WriteData_8Bit(0x3f);
    LCD_WriteData_8Bit(0x00);
    LCD_WriteData_8Bit(0x07);
    LCD_WriteData_8Bit(0x03);
    LCD_WriteData_8Bit(0x10);

    LCD_WriteReg(0xF0); //Enable test command
    LCD_WriteData_8Bit(0x01);

    LCD_WriteReg(0xF6); //Disable ram power save mode
    LCD_WriteData_8Bit(0x00);

    LCD_WriteReg(0x3A); //65k mode
    LCD_WriteData_8Bit(0x05);

}


/********************************************************************************
function:	Set the display scan and color transfer modes
parameter:
		Scan_dir   :   Scan direction
		Colorchose :   RGB or GBR color format
********************************************************************************/
void LCD_SetGramScanWay(LCD_SCAN_DIR Scan_dir)
{
    //Get the screen scan direction
    sLCD_DIS.LCD_Scan_Dir = Scan_dir;

    //Get GRAM and LCD width and height
    if(Scan_dir == L2R_U2D || Scan_dir == L2R_D2U || Scan_dir == R2L_U2D || Scan_dir == R2L_D2U) {
        sLCD_DIS.LCD_Dis_Column	= LCD_HEIGHT ;
        sLCD_DIS.LCD_Dis_Page = LCD_WIDTH ;
    } else {
        sLCD_DIS.LCD_Dis_Column	= LCD_WIDTH ;
        sLCD_DIS.LCD_Dis_Page = LCD_HEIGHT ;
    }

    // Gets the scan direction of GRAM
    uint16_t MemoryAccessReg_Data=0;  //0x36
    switch (Scan_dir) {
    case L2R_U2D:
        MemoryAccessReg_Data = 0X00 | 0x00;//x Scan direction | y Scan direction
        break;
    case L2R_D2U:
        MemoryAccessReg_Data = 0x00 | 0x80;//0xC8 | 0X10
        break;
    case R2L_U2D://	0X4
        MemoryAccessReg_Data = 0x40 | 0x00;
        break;
    case R2L_D2U://	0XC
        MemoryAccessReg_Data = 0x40 | 0x80;
        break;
    case U2D_L2R://0X2
        MemoryAccessReg_Data = 0X00 | 0X00 | 0x20;
        break;
    case U2D_R2L://0X6
        MemoryAccessReg_Data = 0x00 | 0X40 | 0x20;
        break;
    case D2U_L2R://0XA
        MemoryAccessReg_Data = 0x80 | 0x00 | 0x20;
        break;
    case D2U_R2L://0XE
        MemoryAccessReg_Data = 0x40 | 0x80 | 0x20;
        break;
    }

    //please set (MemoryAccessReg_Data & 0x10) != 1
    if((MemoryAccessReg_Data && 0x20) != 1) {
        sLCD_DIS.LCD_X_Adjust = LCD_X;
        sLCD_DIS.LCD_Y_Adjust = LCD_Y;
    } else {
        sLCD_DIS.LCD_X_Adjust = LCD_Y;
        sLCD_DIS.LCD_Y_Adjust = LCD_X;
    }

    // Set the read / write scan direction of the frame memory
    LCD_WriteReg(0x36); //MX, MY, RGB mode
#if defined(LCD_1IN44)
    LCD_WriteData_8Bit( MemoryAccessReg_Data | 0x08);	//0x08 set RGB
#elif defined(LCD_1IN8)
    LCD_WriteData_8Bit( MemoryAccessReg_Data & 0xf7);	//RGB color filter panel
#endif

}



