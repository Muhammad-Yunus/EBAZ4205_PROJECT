# Project Instruction
- To get started, 
    - Export Hardware Platform from [Vivado Project - HDMI Passthrough](../../Vivado/6_HDMI_Passthrough/EBAZ4205_6_HDMI_Passthrough.vivado/)
    - Launch Xilinx SDK from Vivado,
    - Then create New Application Project with Name `HDMI_Passthroug_Test`,
    - Choose Project Template `Hello World`
    - <font color="orange">Leave the helloworld.c as is</font>, since we use the code to activate PS.
        - So it can supply clock to the PL.
    - Build and upload to EBAZ4205 using Xilinx Platform USB Cable.
- **HDMI OUT** Pin :
    - `TMDS Pin` in HDMI / DVI interface, 
        - `TMDS 0` = `BLUE` Cable
        - `TMDS 1` = `GREEN` Cable
        - `TMDS 2` = `RED` Cable
        - `TMDS CLK` = `BROWN` Cable
    - `HPD Pin` for Hot Plug Detection
        - ⚠️⚠️⚠️<font color="orange">Use Level Shifter 3.3V to 5V.</font>
        - EBAZ4205 <font color="orange">HPD Pin (3.3v)</font> --> Level Shifter 3.3V to 5V --> HDMI Out Cable <font color="orange">HPD Pin (5V)</font>.
- **HDMI IN** Pin :
    - `TMDS Pin` in HDMI / DVI interface, 
        - `TMDS 0` = `BLUE` Cable
        - `TMDS 1` = `GREEN` Cable
        - `TMDS 2` = `RED` Cable
        - `TMDS CLK` = `BROWN` Cable
    - `HPD Pin` for Hot Plug Detection
        - ⚠️⚠️⚠️<font color="orange">Use Level Shifter 3.3V to 5V.</font>
        - EBAZ4205 <font color="orange">HPD Pin (3.3v)</font> --> Level Shifter 3.3V to 5V --> HDMI In Cable <font color="orange">HPD Pin (5V)</font>.
    - `DDC Pin` (SDA, SCL) for Display Data Channel (detect resolution, etc.)
        - ⚠️⚠️⚠️<font color="orange">Use Level Shifter 3.3V to 5V.</font>
        - EBAZ4205 <font color="orange">DDC SDA (3.3v)</font> --> Level Shifter 3.3V to 5V --> HDMI In Cable <font color="orange">DDC SDA (5V)</font>.
        - EBAZ4205 <font color="orange">DDC SCL (3.3v)</font> --> Level Shifter 3.3V to 5V --> HDMI In Cable <font color="orange">DDC SCL (5V)</font>.
        <img src="../../resource/EBAZ4205_6_HDMI_Passthrough_Pin.png" width="100%"><br><br>
            <p align="left">
                <img src="../../resource/hdmi_pinout.jpg" height="250">
                <img src="../../resource/level_shifter.jpg" height="250">
            </p>
- Experiment result,
    - HDMI Out Laptop -> HDMT RX Cable -> EBAZ4205 -> HDMI TX Cable -> HDMI In External Monitor.<br><br>
    ![](../../resource/EBAZ4205_6_HDMI_Passthrough_Photo.jpeg)
