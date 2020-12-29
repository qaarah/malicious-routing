
#Example test for and gate. 

/home/qazi/Desktop/Qazi/tools/vtr/yosys_0.7/./yosys -p 'synth_ice40 -top top -blif mux_example.blif' example_mux1.v 2>&1 
# yosys -p  "read_verilog example_mux1.v; hierarchy -top top; setattr -set keep 1; proc; opt; memory; opt; fsm; opt; techmap; opt; abc -lut 4; opt; write_blif mux_example.blif " 2>&1 
# yosys -p  'synth_ice40 -top top -blif mux_example.blif; show' example_mux1.v 2>&1
#/home/qazi/Desktop/Qazi/arachne-pnr_Infected/bin/./arachne-pnr -d 1k -p /home/qazi/Desktop/Qazi/icestorm/examples/icestick/icestick.pcf -o example_MLUT.asc example_MLUT.blif 2>&1
arachne-pnr -d 1k -o mux_example.asc -p mux1_example.pcf mux_example.blif 2>&1
icebox_explain mux_example.asc 2>&1 | tee mux_example_explain.txt
icepack mux_example.asc mux_example.bin  2>&1
# iceunpack -vv mux_example.bin 2>&1 | tee unpack_bin.v
iceprog mux_example.bin
icebox_vlog -p mux1_example.pcf mux_example.asc 2>&1 | tee mux_example_re.v
