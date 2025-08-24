#include "GPIO.h"

static u32 gpio_state = 0;

int Gpio_Init(XGpio *InstancePtr) {
	int Status;

    Status = XGpio_Initialize(InstancePtr, XPAR_AXI_GPIO_0_DEVICE_ID);
    if (Status != XST_SUCCESS)
    	return XST_FAILURE;
    XGpio_SetDataDirection(InstancePtr, GPIO_CHANNEL, 0x0); // All outputs
    gpio_state = 0;
    XGpio_DiscreteWrite(InstancePtr, GPIO_CHANNEL, gpio_state);

    return XST_SUCCESS;
}

void Gpio_High(XGpio *InstancePtr, u32 mask) {
    gpio_state |= mask;
    XGpio_DiscreteWrite(InstancePtr, GPIO_CHANNEL, gpio_state);
}

void Gpio_Low(XGpio *InstancePtr, u32 mask) {
    gpio_state &= ~mask;
    XGpio_DiscreteWrite(InstancePtr, GPIO_CHANNEL, gpio_state);
}

void Gpio_Toggle(XGpio *InstancePtr, u32 mask) {
    gpio_state ^= mask;
    XGpio_DiscreteWrite(InstancePtr, GPIO_CHANNEL, gpio_state);
}
