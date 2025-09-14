# Project Instruction
- The `src/` folder contain application code for running Test Pattern Generator (TPG) on DVI interface in Xilinx SDK 2019.1 for EBAZ4205.
- To get started, 
    - Export Hardware Platform from [Vivado Project - HDMI Passthrough](../../Vivado/6_HDMI_Passthrough/EBAZ4205_6_HDMI_Passthrough.vivado/)
    - Launch Xilinx SDK from Vivado,
    - Then create New Application Project with Name `HDMI_Passthroug_Test`,
    - Choose Project Template `Hello World`
    - Leave the helloworld.c as is, since we use the code to activate PS.
        - So it can supply clock to the PL.
    - Build and upload to EBAZ4205 using Xilinx Platform USB Cable.
- HDMI TX Pin :
    - `TMDS Pin` in HDMI / DVI interface, 
        - `TMDS 0` = `BLUE` Cable
        - `TMDS 1` = `GREEN` Cable
        - `TMDS 2` = `RED` Cable
        - `TMDS CLK` = `BROWN` Cable
    - `HPD Pin` for Hot Plug
- HDMI RX Pin :
    - `TMDS Pin` in HDMI / DVI interface, 
        - `TMDS 0` = `BLUE` Cable
        - `TMDS 1` = `GREEN` Cable
        - `TMDS 2` = `RED` Cable
        - `TMDS CLK` = `BROWN` Cable
    - `HPD Pin` for Hot Plug Detection
    - `DDC Pin` (SDA, SCL) for Display Data Channel (detect resolution, etc.)
        <img src="../../resource/EBAZ4205_6_HDMI_Passthrough_Pin.png" width="100%"><br><br>
        <img src="../../resource/hdmi_pinout.jpg" height="250">
- Experiment result,
    - HDMI Out Laptop -> HDMT RX Cable -> EBAZ4205 -> HDMI TX Cable -> HDMI In External Monitor.<br><br>
    ![](../../../resource/EBAZ4205_6_HDMI_Passthrough_Photo.jpeg)