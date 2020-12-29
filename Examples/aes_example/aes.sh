
#**********************************************************
# Aes Implementation with original Tool Flow
#**********************************************************
# yosys -p "read_verilog aes_top.v key_expansion.v mixcolumn.v byte_permutation_unit.v aes_data_path.v sbox_case_4.v parallel_serial_converter.v mux.v; synth_ice40 -blif aes_select.blif" 2>&1
arachne-pnr -d 1k -o aes_orig.asc -p aes_icestick.pcf original_aes/aes.blif 2>&1
# # icebox_explain aes.asc 2>&1 | tee aes_explain.txt
icepack aes_orig.asc aes_orig.bin  2>&1
# # # iceunpack -vv aes.bin 2>&1 | tee unpack_bin.v
iceprog aes_orig.bin
icetime -tmd hx1k aes_orig.asc 2>&1 | tee timing_report_org.txt
# # # icebox_vlog -p example.pcf aes.asc 2>&1 | tee my_example_re.v


#**********************************************************
# Aes Implementation with original Tool Flow SB_LUT
#**********************************************************
# yosys -p "read_verilog aes_top.v key_expansion.v mixcolumn.v byte_permutation_unit.v aes_data_path.v sbox_case_4.v parallel_serial_converter.v mux.v; synth_ice40 -blif aes_LUT.blif" 2>&1
# arachne-pnr -d 1k -o aes_LUT.asc -p aes_icestick.pcf aes_LUT.blif 2>&1
# # icebox_explain aes.asc 2>&1 | tee aes_explain.txt
# icepack aes_LUT.asc aes.bin  2>&1
# # # iceunpack -vv aes.bin 2>&1 | tee unpack_bin.v
# iceprog aes.bin
# # icebox_vlog -p example.pcf aes.asc 2>&1 | tee my_example_re.v

#**********************************************************
# Aes Implementation with original Tool Flow SB_LUT Modified
#**********************************************************
# arachne-pnr -d 1k -o ./modified_aes_blif/Modified_aes.asc -p aes_icestick.pcf ./modified_aes_blif/modified_aes.blif 2>&1
# icebox_explain aes.asc 2>&1 | tee aes_explain.txt
# icepack ./modified_aes_blif/Modified_aes.asc ./modified_aes_blif/aes.bin  2>&1
# # # iceunpack -vv aes.bin 2>&1 | tee unpack_bin.v
# iceprog ./modified_aes_blif/aes.bin
# icebox_vlog -p aes_icestick.pcf ./modified_aes_blif/Modified_aes.asc 2>&1 | tee ./modified_aes_blif/Modified_aes_re.v

#**********************************************************
# Aes Implementation with Malicious Tool Flow
#**********************************************************
yosys -p "read_verilog aes_top.v key_expansion.v mixcolumn.v byte_permutation_unit.v aes_data_path.v sbox_case_4.v parallel_serial_converter.v mux.v; synth_ice40 -blif aes_trojan.blif" 2>&1
/home/qazi/Desktop/icefpga_flow/Infected_icefpga_flow/tools/arachne-pnr/bin/./arachne-pnr -d 1k -o aes_trojan.asc -p aes_icestick.pcf aes_trojan.blif 2>&1
# icebox_explain aes_trojan.asc 2>&1 | tee aes_trojan_explain.txt
icepack aes_trojan.asc aes_trojan.bin  2>&1
# iceunpack -vv aes_trojan.bin 2>&1 | tee unpack_bin.v
/home/qazi/Desktop/icefpga_flow/Infected_icefpga_flow/tools/icestorm/iceprog/./iceprog aes_trojan.bin
iceprog aes_trojan.bin
icetime -tmd hx1k aes_trojan.asc | tee timing_report_Tro.txt
# icebox_vlog -p aes_icestick.pcf aes_trojan.asc 2>&1 | tee aes_trojan_re.v