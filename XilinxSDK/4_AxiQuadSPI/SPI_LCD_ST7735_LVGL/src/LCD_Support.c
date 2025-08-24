#include "LCD_Support.h"

extern XGpio gpio0;
extern XSpi  SpiInstance;
extern LCD_DIS sLCD_DIS;

static uint8_t s_frameBuffer[2][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL];

int LCD_Init( LCD_SCAN_DIR Lcd_ScanDir )
{
	int Status;

    /* Initialize the GPIO 0 driver */
	Status = Gpio_Init(&gpio0);
	if (Status != XST_SUCCESS) {
		xil_printf("Gpio 0 Initialization Failed\r\n");
		return XST_FAILURE;
	}

	/* Set up the AXI SPI Controller */
	Status = XSpi_Init(&SpiInstance, SPI_DEVICE_ID);
	if (Status != XST_SUCCESS) {
		xil_printf("SPI Mode Failed\r\n");
		return XST_FAILURE;
	}

    // turn on LCD backlight
    Gpio_High(&gpio0, GPIO_BACKLIGHT);

    //Hardware reset
    LCD_Reset();

    //Set the initialization register
    LCD_InitReg();

    //Set the display scan and color transfer modes
    LCD_SetGramScanWay( Lcd_ScanDir );
    delay_ms(200);

    //sleep out
    LCD_WriteReg(0x11);
    delay_ms(120);

    //Turn on the LCD display
    LCD_WriteReg(0x29);

    return XST_SUCCESS;
}

void LCD_SetWindows( POINT Xstart, POINT Ystart, POINT Xend, POINT Yend )
{

    //set the X coordinates
    LCD_WriteReg ( 0x2A );
    LCD_WriteData_8Bit ( 0x00 );						//Set the horizontal starting point to the high octet
    LCD_WriteData_8Bit ( (Xstart & 0xff) + sLCD_DIS.LCD_X_Adjust);			//Set the horizontal starting point to the low octet
    LCD_WriteData_8Bit ( 0x00 );				//Set the horizontal end to the high octet
    LCD_WriteData_8Bit ( (( Xend - 1 ) & 0xff) + sLCD_DIS.LCD_X_Adjust);	//Set the horizontal end to the low octet

    //set the Y coordinates
    LCD_WriteReg ( 0x2B );
    LCD_WriteData_8Bit ( 0x00 );
    LCD_WriteData_8Bit ( (Ystart & 0xff) + sLCD_DIS.LCD_Y_Adjust);
    LCD_WriteData_8Bit ( 0x00 );
    LCD_WriteData_8Bit ( ( (Yend - 1) & 0xff )+ sLCD_DIS.LCD_Y_Adjust);

    LCD_WriteReg(0x2C);

}

static void LCD_FlushDisplay(lv_disp_drv_t *drv, const lv_area_t *a, lv_color_t *color_p)
{
    uint32_t num_pixels  = (a->x2 - a->x1 + 1) * (a->y2 - a->y1 + 1);
    LCD_SetWindows(a->x1, a->y1, a->x2+1, a->y2+1);

    // Turn DC to High for sending data
    Gpio_High(&gpio0, GPIO_DC);

    static uint8_t txbuf[LDC_TRX_SLICE_PIXELS * 2];

    uint32_t sent = 0;
    while (sent < num_pixels) {
        uint32_t n = num_pixels - sent;
        if (n > LDC_TRX_SLICE_PIXELS) n = LDC_TRX_SLICE_PIXELS;

        // Convert n pixels from LVGL memory order to [hi][lo] for SPI
        for (uint32_t i = 0; i < n; i++) {
            uint16_t c = color_p[sent + i].full;   // RGB565
            txbuf[2*i]     = (uint8_t)(c >> 8);    // high byte first
            txbuf[2*i + 1] = (uint8_t)(c & 0xFF);  // low byte
        }
        XSpi_Transfer(&SpiInstance, txbuf, NULL, n * 2);
        sent += n;
    }
    lv_disp_flush_ready(drv);
}

void lv_port_disp_init(void)
{
    LCD_Init(L2R_U2D);

    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, s_frameBuffer[0], s_frameBuffer[1], LCD_VIRTUAL_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = LCD_FlushDisplay;
    disp_drv.draw_buf = &draw_buf;
    //disp_drv.full_refresh = 0; // optional, set 1 if needed

    lv_disp_drv_register(&disp_drv);
}
