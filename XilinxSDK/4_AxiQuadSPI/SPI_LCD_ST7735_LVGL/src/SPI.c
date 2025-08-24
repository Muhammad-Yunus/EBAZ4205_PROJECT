
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



void SPI_Write(XSpi *SpiPtr, unsigned char data)
{
	u8 TxBuffer[1] = {0};
	u8 RxBuffer[1] = {0};

	TxBuffer[0] = data;

	XSpi_Transfer(SpiPtr, TxBuffer, RxBuffer, (int)1);
}


void SPI_WriteWord(XSpi *SpiPtr, unsigned int data)
{
	u8 TxBuffer[2];
	u8 RxBuffer[2];

	TxBuffer[0] = data>>8;
	TxBuffer[1] = data;

	XSpi_Transfer(SpiPtr, TxBuffer, RxBuffer, 2);
}

void SPI_WriteWord32(XSpi *SpiPtr, uint32_t word)
{
    u8 TxBuffer[4];
    u8 RxBuffer[4];

    TxBuffer[0] = (word >> 24) & 0xFF; // MSB first
    TxBuffer[1] = (word >> 16) & 0xFF;
    TxBuffer[2] = (word >> 8) & 0xFF;
    TxBuffer[3] = word & 0xFF;

    XSpi_Transfer(SpiPtr, TxBuffer, RxBuffer, 4);
}

void SPI_WriteWord64(XSpi *SpiPtr, uint64_t word)
{
    u8 TxBuffer[8];
    u8 RxBuffer[8];

    TxBuffer[0] = (word >> 56) & 0xFF; // MSB first
	TxBuffer[1] = (word >> 48) & 0xFF;
	TxBuffer[2] = (word >> 40) & 0xFF;
	TxBuffer[3] = (word >> 32) & 0xFF;
    TxBuffer[4] = (word >> 24) & 0xFF;
    TxBuffer[5] = (word >> 16) & 0xFF;
    TxBuffer[6] = (word >> 8) & 0xFF;
    TxBuffer[7] = word & 0xFF;

    XSpi_Transfer(SpiPtr, TxBuffer, RxBuffer, 8);
}
