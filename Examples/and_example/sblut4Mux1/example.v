module top (input a,b,s,output y);
wire y1;
assign y1 = a & b;	
localparam [14:0] KEEP = 15'b110011001111000;

SB_LUT4 #(.LUT_INIT({ KEEP, 1'b0})) magic_zro_zro (.I0(1'b0), .I1(b), .I2(y1), .I3(s), .O(y));
endmodule

