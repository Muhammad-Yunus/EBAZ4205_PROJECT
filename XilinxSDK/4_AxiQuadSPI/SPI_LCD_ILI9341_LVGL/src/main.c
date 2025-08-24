/******************************************************************************
*
* Copyright (C) 2009 - 2014 Xilinx, Inc.  All rights reserved.
*
* Permission is hereby granted, free of charge, to any person obtaining a copy
* of this software and associated documentation files (the "Software"), to deal
* in the Software without restriction, including without limitation the rights
* to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
* copies of the Software, and to permit persons to whom the Software is
* furnished to do so, subject to the following conditions:
*
* The above copyright notice and this permission notice shall be included in
* all copies or substantial portions of the Software.
*
* Use of the Software is limited solely to applications:
* (a) running on a Xilinx device, or
* (b) that interact with a Xilinx device through a bus or interconnect.
*
* THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
* IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
* FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL
* XILINX  BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY,
* WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF
* OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
* SOFTWARE.
*
* Except as contained in this notice, the name of the Xilinx shall not be used
* in advertising or otherwise to promote the sale, use or other dealings in
* this Software without prior written authorization from Xilinx.
*
******************************************************************************/

/*
 * SPI LCF ILI9341 + LVGL Demo
 */

#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xscutimer.h"
#include "xscugic.h"
#include "xil_exception.h"
#include "lvgl.h"
#include "demos/lv_demos.h"
#include "LCD_Support.h"

#define TIMER_DEVICE_ID     XPAR_XSCUTIMER_0_DEVICE_ID
#define INTC_DEVICE_ID      XPAR_SCUGIC_SINGLE_DEVICE_ID
#define TIMER_IRPT_INTR     XPAR_SCUTIMER_INTR

static XScuTimer Timer;    // Timer instance
static XScuGic Intc;       // Interrupt controller instance

int systick_init();
static void TimerIntrHandler(void *CallBackRef);
void lvgl_test_screen(void);
void create_rect_with_anim(void);

int main()
{
    init_platform();

    systick_init();

    lv_init();
	lv_port_disp_init();

	//lvgl_test_screen();
	//create_rect_with_anim();
	//lv_demo_widgets();
	lv_demo_music();

	while(1)
	{
		lv_timer_handler();
		usleep(2000);  // ~2 ms delay, or use a busy-wait
	}
    cleanup_platform();
    return 0;
}

/****************************LVGL DEMO****************************/
void lvgl_test_screen(void)
{
    /* Create a button on the active screen */
    lv_obj_t * btn = lv_btn_create(lv_scr_act());
    lv_obj_align(btn, LV_ALIGN_CENTER, 0, 0);  // Center of screen
    lv_obj_set_size(btn, 100, 50);             // 100x50 px

    /* Add a label on the button */
    lv_obj_t * label = lv_label_create(btn);
    lv_label_set_text(label, "Click me");
    lv_obj_center(label);
}

static void anim_color_cb(void * obj, int32_t v)
{
    // `v` will go from 0 (red) to 255 (blue)
    lv_color_t c = lv_color_mix(lv_palette_main(LV_PALETTE_BLUE),
                                lv_palette_main(LV_PALETTE_RED),
                                v); // v = 0 -> RED, v=255 -> BLUE
    lv_obj_set_style_bg_color((lv_obj_t *)obj, c, LV_PART_MAIN);
}

void create_rect_with_anim(void)
{
    // Create rectangle
    lv_obj_t * rect = lv_obj_create(lv_scr_act());
    lv_obj_set_size(rect, 100, 100);
    lv_obj_center(rect);

    // Remove default style if needed (optional)
    lv_obj_set_style_radius(rect, 0, LV_PART_MAIN);

    // Set initial color
    lv_obj_set_style_bg_color(rect, lv_palette_main(LV_PALETTE_RED), LV_PART_MAIN);

    // Create animation
    lv_anim_t a;
    lv_anim_init(&a);
    lv_anim_set_var(&a, rect);
    lv_anim_set_exec_cb(&a, anim_color_cb);
    lv_anim_set_time(&a, 500); // 500 ms
    lv_anim_set_values(&a, 0, 255); // From RED to BLUE
    lv_anim_set_repeat_count(&a, LV_ANIM_REPEAT_INFINITE);
    lv_anim_set_playback_time(&a, 500); // Animate back to red

    lv_anim_start(&a);
}

/*****************************TIMER HANLDER******************************/

static void TimerIntrHandler(void *CallBackRef)
{
    XScuTimer_ClearInterruptStatus(&Timer);
    lv_tick_inc(1);   // tell LVGL 1 ms passed
}
int systick_init()
{
    int Status;
    XScuTimer_Config *ConfigPtr;
    XScuGic_Config *GicConfig;

    // Initialize timer
    ConfigPtr = XScuTimer_LookupConfig(TIMER_DEVICE_ID);
    Status = XScuTimer_CfgInitialize(&Timer, ConfigPtr, ConfigPtr->BaseAddr);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    XScuTimer_SelfTest(&Timer);
    XScuTimer_EnableAutoReload(&Timer);

    // Load for 1ms interval: CPU_Clk/Prescaler/1000
    XScuTimer_LoadTimer(&Timer, XPAR_CPU_CORTEXA9_0_CPU_CLK_FREQ_HZ / 1000);

    // Initialize interrupt controller
    GicConfig = XScuGic_LookupConfig(INTC_DEVICE_ID);
    Status = XScuGic_CfgInitialize(&Intc, GicConfig, GicConfig->CpuBaseAddress);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    // Connect timer interrupt
    Status = XScuGic_Connect(&Intc, TIMER_IRPT_INTR,
                             (Xil_ExceptionHandler)TimerIntrHandler,
                             (void *)&Timer);
    if (Status != XST_SUCCESS) return XST_FAILURE;

    XScuGic_Enable(&Intc, TIMER_IRPT_INTR);

    // Enable interrupts in CPU
    Xil_ExceptionInit();
    Xil_ExceptionRegisterHandler(XIL_EXCEPTION_ID_IRQ_INT,
                                 (Xil_ExceptionHandler)XScuGic_InterruptHandler,
                                 &Intc);
    Xil_ExceptionEnable();

    // Start timer
    XScuTimer_EnableInterrupt(&Timer);
    XScuTimer_Start(&Timer);

    return XST_SUCCESS;
}
