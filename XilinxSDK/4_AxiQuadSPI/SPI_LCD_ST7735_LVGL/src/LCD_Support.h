/*
 * GUI.h
 *
 *  Created on: Aug 23, 2025
 *      Author: yunus
 */

#ifndef SRC_LCD_SUPPORT_H_
#define SRC_LCD_SUPPORT_H_

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "lvgl.h"
#include "LCD_Driver.h"

#define LCD_WIDTH             	128
#define LCD_HEIGHT            	128
#define LCD_FB_BYTE_PER_PIXEL 	2
#define LCD_VIRTUAL_BUF_SIZE 	(LCD_WIDTH * LCD_HEIGHT)
#define LDC_TRX_SLICE_PIXELS  	128

#ifdef __cplusplus
extern "C" {
#endif

void lv_port_disp_init(void);

#if defined(__cplusplus)
}
#endif

#endif /* SRC_LCD_SUPPORT_H_ */
