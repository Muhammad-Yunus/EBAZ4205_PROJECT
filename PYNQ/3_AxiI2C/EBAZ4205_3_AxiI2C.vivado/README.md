### Create `EBAZ4205_3_AxiI2C` Project
- Create a New Vivado Project with name `EBAZ4205_3_AxiI2C`,
- Copy `EBAZ4205_3_AxiI2C.tcl` above into `EBAZ4205_3_AxiI2C` project folder using file explorer,
- Open TCL Console in Vivado (bottom section, first tab), run this command,
```
source EBAZ4205_3_AxiI2C.tcl
```
- Block design will be generated.
- source the `io.xdc` as constraint file.
- Run Synthesis -> Implementation -> Generate Bitstream.
- Find `.bit`, `.tcl` & `.hwh` files inside `EBAZ4205_3_AxiI2C` project folder.
- Copy that and upload into `~/pynq/overlays/` folder of PYNQ OS running in EBAZ4205.