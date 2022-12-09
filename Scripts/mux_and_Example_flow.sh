
#Example test for and gate. 

yosys -p 'synth_ice40 -top top -blif mux_example.blif' example_mux1.v 2>&1 
arachne-pnr -d 1k -o mux_example.asc -p mux1_example.pcf mux_example.blif 2>&1
icebox_explain mux_example.asc 2>&1 | tee mux_example_explain.txt
icepack mux_example.asc mux_example.bin  2>&1
# iceunpack -vv mux_example.bin 2>&1 | tee unpack_bin.v
iceprog mux_example.bin
icebox_vlog -p mux1_example.pcf mux_example.asc 2>&1 | tee mux_example_re.v
