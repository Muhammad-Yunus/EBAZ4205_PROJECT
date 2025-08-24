#include "LCD_Support.h"

XSpi  SpiInstance;
XGpio gpio0;

static uint8_t s_frameBuffer[2][LCD_VIRTUAL_BUF_SIZE * LCD_FB_BYTE_PER_PIXEL];

void LCD_WriteCmd(uint8_t cmd)
{
	Gpio_Low(&gpio0, GPIO_DC);
	XSpi_Write(&SpiInstance, cmd);
}

void LCD_WriteData(uint8_t data)
{
	Gpio_High(&gpio0, GPIO_DC);
	XSpi_Write(&SpiInstance, data);
}

void LCD_WriteMultiData(const uint8_t *data, size_t length)
{
	Gpio_High(&gpio0, GPIO_DC);

	XSpi_Transfer(&SpiInstance, (u8 *)data, NULL, length);
}

void LCD_Reset(void)
{
    Gpio_High(&gpio0, GPIO_RESET);
    delay_ms(100);
    Gpio_Low(&gpio0, GPIO_RESET);
    delay_ms(100);
    Gpio_High(&gpio0, GPIO_RESET);
    delay_ms(100);
}

int LCD_Init()
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

    /* Turn on LCD backlight */
    Gpio_High(&gpio0, GPIO_BACKLIGHT);

    /* Hardware reset */
    LCD_Reset();

    /* Set the initialization register */
    ILI9341_Init(LCD_WriteData, LCD_WriteCmd);

    /* Change to landscape view. */
	LCD_WriteCmd(ILI9341_CMD_MAC);
	LCD_WriteData(0x28);

    return XST_SUCCESS;
}

static void LCD_FlushDisplay(lv_disp_drv_t *disp_drv, const lv_area_t *area, lv_color_t *color_p)
{
	lv_coord_t x1 = area->x1;
	lv_coord_t y1 = area->y1;
	lv_coord_t x2 = area->x2;
	lv_coord_t y2 = area->y2;

	uint8_t data[4];
	uint32_t px_size = (x2 - x1 + 1) * (y2 - y1 + 1);

	/*Column addresses*/
	LCD_WriteCmd(ILI9341_CMD_COLADDR);
	data[0] = (x1 >> 8) & 0xFF;
	data[1] = x1 & 0xFF;
	data[2] = (x2 >> 8) & 0xFF;
	data[3] = x2 & 0xFF;
	LCD_WriteMultiData(data, 4);

	/*Page addresses*/
	LCD_WriteCmd(ILI9341_CMD_PAGEADDR);
	data[0] = (y1 >> 8) & 0xFF;
	data[1] = y1 & 0xFF;
	data[2] = (y2 >> 8) & 0xFF;
	data[3] = y2 & 0xFF;
	LCD_WriteMultiData(data, 4);

	/*Memory write*/
	LCD_WriteCmd(ILI9341_CMD_GRAM);

	LCD_WriteMultiData((const uint8_t *)color_p, px_size * LCD_FB_BYTE_PER_PIXEL);

	lv_disp_flush_ready(disp_drv);
}

void lv_port_disp_init(void)
{
    LCD_Init();

    static lv_disp_draw_buf_t draw_buf;
    lv_disp_draw_buf_init(&draw_buf, s_frameBuffer[0], s_frameBuffer[1], LCD_VIRTUAL_BUF_SIZE);

    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);
    disp_drv.hor_res = LCD_WIDTH;
    disp_drv.ver_res = LCD_HEIGHT;
    disp_drv.flush_cb = LCD_FlushDisplay;
    disp_drv.draw_buf = &draw_buf;
    disp_drv.full_refresh = 0; // optional, set 1 if needed

    lv_disp_drv_register(&disp_drv);
}
