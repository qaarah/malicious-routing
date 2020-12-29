
#**********************************************************
# Aes Implementation with original Tool Flow
#**********************************************************
# yosys -p "read_verilog aes_top.v key_expansion.v mixcolumn.v byte_permutation_unit.v aes_data_path.v sbox_case_4.v parallel_serial_converter.v mux.v; synth_ice40 -blif aes_select.blif" 2>&1
# arachne-pnr -d 1k -o aes_select.asc -p aes_icestick.pcf aes_select.blif 2>&1
# # icebox_explain aes.asc 2>&1 | tee aes_explain.txt
# icepack aes_select.asc aes.bin  2>&1
# # # iceunpack -vv aes.bin 2>&1 | tee unpack_bin.v
# iceprog aes.bin
# # # icebox_vlog -p example.pcf aes.asc 2>&1 | tee my_example_re.v


#**********************************************************
# Aes Implementation with original Tool Flow SB_LUT
#**********************************************************
# yosys -p "read_verilog ./verilog/aes_top.v ./verilog/key_expansion.v ./verilog/mixcolumn.v ./verilog/byte_permutation_unit.v ./verilog/aes_data_path.v ./verilog/sbox_case_4.v ./verilog/parallel_serial_converter.v ./verilog/mux.v; synth_ice40 -blif aes_LUT.blif" 2>&1
# arachne-pnr -d 1k -o aes_LUT.asc -p aes_icestick.pcf aes_LUT.blif 2>&1
# # # icebox_explain aes.asc 2>&1 | tee aes_explain.txt
# # icepack aes_LUT.asc aes.bin  2>&1
# # # # iceunpack -vv aes.bin 2>&1 | tee unpack_bin.v
# # iceprog aes.bin
# icebox_vlog -p aes_icestick.pcf aes_LUT.asc 2>&1 | tee my_example_re.v

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
# Aes Implementation with original Tool Flow with select in verilog
#**********************************************************
yosys -p "read_verilog ./verilog/aes_top.v ./verilog/key_expansion.v ./verilog/mixcolumn.v ./verilog/byte_permutation_unit.v ./verilog/aes_data_path.v ./verilog/sbox_case_4.v ./verilog/parallel_serial_converter.v ./verilog/mux.v; synth_ice40 -blif aes_select.blif" 2>&1
arachne-pnr -d 1k -o aes_select.asc -p aes_icestick.pcf aes_select.blif 2>&1
# # icebox_explain aes.asc 2>&1 | tee aes_explain.txt
# icepack aes_select.asc aes.bin  2>&1
# # # iceunpack -vv aes.bin 2>&1 | tee unpack_bin.v
# iceprog aes.bin
icebox_vlog -p aes_icestick.pcf aes_select.asc 2>&1 | tee my_example_re.v