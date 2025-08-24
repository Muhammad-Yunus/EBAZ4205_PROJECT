/*
 * GPIO.h
 *
 *  Created on: Aug 23, 2025
 *      Author: yunus
 */

#ifndef SRC_GPIO_H_
#define SRC_GPIO_H_
#include "xgpio.h"


#define GPIO_DEVICE_ID	XPAR_AXI_GPIO_0_DEVICE_ID

#define GPIO_DC       	(1 << 0)
#define GPIO_BACKLIGHT 	(1 << 1)
#define GPIO_RESET    	(1 << 2)
#define GPIO_CHANNEL 	1

int Gpio_Init(XGpio *InstancePtr);
void Gpio_High(XGpio *InstancePtr, u32 mask);
void Gpio_Low(XGpio *InstancePtr, u32 mask);
void Gpio_Toggle(XGpio *InstancePtr, u32 mask);


#endif /* SRC_GPIO_H_ */
