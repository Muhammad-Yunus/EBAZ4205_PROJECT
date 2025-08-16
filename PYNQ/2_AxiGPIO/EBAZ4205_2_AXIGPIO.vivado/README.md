### Create `EBAZ4205_2_AXIGPIO` Project
- Create a New Vivado Project with name `EBAZ4205_2_AXIGPIO`,
- Copy `EBAZ4205_2_AXIGPIO.tcl` above into `EBAZ4205_2_AXIGPIO` project folder using file explorer,
- Open TCL Console in Vivado (bottom section, first tab), run this command,
```
source EBAZ4205_2_AXIGPIO.tcl
```
- Block design will be generated.
- Run Synthesis -> Implementation -> Generate Bitstream
- Find `.bit`, `.tcl` & `.hwh` files inside `EBAZ4205_2_AXIGPIO` project folder.
- Copy that and upload into `~/pynq/overlays/` folder of PYNQ OS running in EBAZ4205.