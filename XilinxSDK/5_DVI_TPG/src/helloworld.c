#include <stdio.h>
#include "platform.h"
#include "xil_printf.h"
#include "xv_tpg.h"

XV_tpg tpg_inst;
int Status;

int main()
{
    init_platform();

    print("Hello World\n\r");

    // Initialize the TPG (replace DeviceId with actual ID)
    Status = XV_tpg_Initialize(&tpg_inst, XPAR_V_TPG_0_DEVICE_ID);
    if (Status != XST_SUCCESS) {
        xil_printf("TPG Initialization Failed!\r\n");
        return XST_FAILURE;
    }

    // Verify if TPG is ready
    if (XV_tpg_IsReady(&tpg_inst)) {
        xil_printf("TPG is ready!\r\n");
    } else {
        xil_printf("TPG is NOT ready!\r\n");
        return XST_FAILURE;
    }

    // Set Resolution to 800x600
	XV_tpg_Set_height(&tpg_inst, 600);
	XV_tpg_Set_width(&tpg_inst, 800);

	// Set Color Space to RGB
	XV_tpg_Set_colorFormat(&tpg_inst, 0x0);

	//Set pattern to color bar
	XV_tpg_Set_bckgndId(&tpg_inst, XTPG_BKGND_CROSS_HATCH);

	// ENABLE MOVING BOX OVERLAY
	XV_tpg_Set_ovrlayId(&tpg_inst, 1);

    // Set Moving Box Properties
    XV_tpg_Set_boxSize(&tpg_inst, 50);      // Box size: 50x50 pixels
    XV_tpg_Set_motionSpeed(&tpg_inst, 5);   // Move 5 pixels per frame
    XV_tpg_Set_boxColorR(&tpg_inst, 0x00);
    XV_tpg_Set_boxColorG(&tpg_inst, 0xFF);
    XV_tpg_Set_boxColorB(&tpg_inst, 0x00);

	//Start the TPG
	XV_tpg_EnableAutoRestart(&tpg_inst);
	XV_tpg_Start(&tpg_inst);

    // Print current resolution settings
    xil_printf("TPG width: %d, height: %d\r\n", XV_tpg_Get_width(&tpg_inst), XV_tpg_Get_height(&tpg_inst));

    // Check if TPG is idle, done, or still running
    if (XV_tpg_IsIdle(&tpg_inst)) {
        xil_printf("TPG is idle.\r\n");
    } else {
        xil_printf("TPG is running.\r\n");
    }

    if (XV_tpg_IsDone(&tpg_inst)) {
        xil_printf("TPG has completed operation.\r\n");
    } else {
        xil_printf("TPG is still processing.\r\n");
    }

    cleanup_platform();
    return 0;
}
