
#Example test for and gate. 

yosys_0.7/./yosys -p 'synth_ice40 -top top -blif example.blif' example.v 2>&1 
# yosys -p 'synth_ice40 -top top -blif example.blif' example.v 2>&1 
arachne-pnr -d 1k -o example.asc -p example.pcf example.blif 2>&1
icebox_explain example.asc 2>&1 | tee example_explain.txt
icepack example.asc example.bin  2>&1
# iceunpack -vv example.bin 2>&1 | tee unpack_bin.v
# iceprog example.bin 2>&1 | tee log.txt 
iceprog example.bin 2>&1 | tee modified_icelog.txt
icebox_vlog -p example.pcf example.asc 2>&1 | tee example_re.v

