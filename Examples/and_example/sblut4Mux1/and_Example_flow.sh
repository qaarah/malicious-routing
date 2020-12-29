
#Complete malicious flow for AND Example. 

yosys -p 'synth_ice40 -top top -blif example.blif' example.v 2>&1 
# /home/qazi/Desktop/icefpga_flow/Infected_icefpga_flow/tools/Infected_yosys_0.9/./yosys -p 'synth_ice40 -top top -blif example.blif' example_test.v 2>&1 
# /home/qazi/Desktop/icefpga_flow/Infected_icefpga_flow/tools/arachne-pnr/bin/./arachne-pnr -d 1k -o example.asc -p example.pcf example.blif 2>&1 | tee modified_arachne.txt
arachne-pnr -d 1k -o example.asc -p example.pcf example.blif 2>&1
# icebox_explain example.asc 2>&1 | tee example_explain_asc.txt 
# /home/qazi/Desktop/icefpga_flow/Infected_icefpga_flow/tools/icestorm/icepack/./icepack example.asc example.bin  2>&1
# icepack example.asc example.bin  2>&1
# iceunpack -vv example.bin 2>&1 | tee unpack_bin.v
# iceprog example.bin 2>&1 | tee log.txt 
# /home/qazi/Desktop/icefpga_flow/Infected_icefpga_flow/tools/icestorm/iceprog/./iceprog example.bin 2>&1 | tee modified_iceprog.txt
icebox_vlog -p example.pcf example.asc 2>&1 | tee example_re.v

