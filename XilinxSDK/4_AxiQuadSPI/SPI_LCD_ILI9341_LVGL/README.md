# Project Instruction
- The `src/` folder contain application code for LVGL demo in Xilinx SDK 2019.1 for EBAZ4205.
- Clone LVGL (https://github.com/lvgl/lvgl) into that `src/` folder.
- To get started, 
    - Export Hardware Platform from [Vivado Project - AXI Quad SPI Example](../../../Vivado/4_AxiQuadSPI/EBAZ4205_4_AXIQuadSPI.vivado/)
    - Launch Xilinx SDK from Vivado,
    - Then create New Application Project with Name `SPI_LCD_ILI9341_LVGL`,
    - Choose Project Template `Hello World`
    - Copy everything inside the `src/` folder above into the `src/` folder of your `SPI_LCD_ILI9341_LVGL` project. 
    - Open Project Properties > C/C++ Build > Settings > Directories. 
        - Add `src/lvgl/` as include paths.   
    - Build and upload to EBAZ4205 using Xilinx Platform USB Cable.
- SPI Pin used, 
    - SPI pin is assigned to `A20` (MISO), `H16` (MOSI), `B19` (SCK), `B20` (CS) in `DATA1 PORT`, 
    - Also LCD related GPIO via AxiGPIO, `C20` (LCD DC), `H17` (LCD Backlight), `D20` (LCD RESET)
        <img src="../../../resource/EBAZ4205_4_AXIQuadSPI_Pin.png" width="400px">
- Experiment result,
    - GUI is using **LVGL Demo Music Player**
    - LVGL version : `8.3.10`
    - LCD : Seeed Studio 2.8 Inch TFT LCD Shield V2 240x320 (ILI9341)<br>
    <img src="../../../resource/EBAZ4205_4_AXIQuadSPI_LVGL_1_Photo.jpeg" width="30%">
    <img src="../../../resource/EBAZ4205_4_AXIQuadSPI_LVGL_2_Photo.jpeg" width="30%">
    <img src="../../../resource/EBAZ4205_4_AXIQuadSPI_LVGL_3_Photo.jpeg" width="30%">