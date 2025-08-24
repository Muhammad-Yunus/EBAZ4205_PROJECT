
/**
 * @brief SPI library.
 */

#include "SPI.h"
#include <stdio.h>

/******************************************************************************/
/** XSpi
*
* @param	BaseAddress is the BaseAddress of the SPI device
*
* @return	XST_SUCCESS
*
* @note		None
*
*******************************************************************************/
int XSpi_Init(XSpi *SpiPtr, u16 SpiDeviceId)
{

	int Status;

	XSpi_Config *ConfigPtr;	/* Pointer to Configuration data */

	/*
	 * Initialize the SPI driver so that it is  ready to use.
	 */
	ConfigPtr = XSpi_LookupConfig(SpiDeviceId);
	if (ConfigPtr == NULL) {
		return XST_DEVICE_NOT_FOUND;
	}

	Status = XSpi_CfgInitialize(SpiPtr, ConfigPtr,  ConfigPtr->BaseAddress);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}


	Status = XSpi_SetOptions(SpiPtr, XSP_MASTER_OPTION | XSP_MANUAL_SSELECT_OPTION);
	if (Status != XST_SUCCESS) {
		return XST_FAILURE;
	}

	XSpi_Start(SpiPtr);
	/*
	 * Disable Global interrupt to use polled mode operation
	 */
	XSpi_IntrGlobalDisable(SpiPtr);
	XSpi_SetSlaveSelect(SpiPtr, 0x01);


	return XST_SUCCESS;
}



void XSpi_Write(XSpi *SpiPtr, uint8_t data)
{
	u8 TxBuffer[1] = {0};
	u8 RxBuffer[1] = {0};

	TxBuffer[0] = data;

	XSpi_Transfer(SpiPtr, TxBuffer, RxBuffer, (int)1);
}
