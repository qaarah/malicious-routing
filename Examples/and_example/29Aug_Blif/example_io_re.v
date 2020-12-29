// Reading file 'example_io.asc'..

module chip (output y, input s, input b, input a);

wire y;
// (8, 13, 'sp4_r_v_b_41')
// (8, 14, 'sp4_r_v_b_28')
// (8, 15, 'sp4_r_v_b_17')
// (8, 16, 'sp4_r_v_b_4')
// (9, 12, 'sp4_h_r_4')
// (9, 12, 'sp4_v_t_41')
// (9, 13, 'sp4_v_b_41')
// (9, 14, 'sp4_v_b_28')
// (9, 15, 'sp4_v_b_17')
// (9, 16, 'sp4_h_r_11')
// (9, 16, 'sp4_v_b_4')
// (10, 12, 'sp4_h_r_17')
// (10, 16, 'sp4_h_r_22')
// (11, 12, 'sp4_h_r_28')
// (11, 15, 'neigh_op_tnr_7')
// (11, 16, 'neigh_op_rgt_7')
// (11, 16, 'sp4_h_r_35')
// (11, 17, 'logic_op_bnr_7')
// (12, 12, 'sp4_h_r_41')
// (12, 15, 'neigh_op_top_7')
// (12, 16, 'lutff_7/out')
// (12, 16, 'sp4_h_r_46')
// (12, 17, 'logic_op_bot_7')
// (13, 12, 'io_1/D_OUT_0')
// (13, 12, 'io_1/PAD')
// (13, 12, 'local_g0_1')
// (13, 12, 'span4_horz_41')
// (13, 15, 'logic_op_tnl_7')
// (13, 16, 'logic_op_lft_7')
// (13, 16, 'span4_horz_46')

wire s;
// (10, 16, 'neigh_op_tnr_2')
// (10, 16, 'neigh_op_tnr_6')
// (11, 16, 'neigh_op_top_2')
// (11, 16, 'neigh_op_top_6')
// (11, 17, 'io_1/D_IN_0')
// (11, 17, 'io_1/PAD')
// (12, 16, 'local_g2_2')
// (12, 16, 'lutff_7/in_3')
// (12, 16, 'neigh_op_tnl_2')
// (12, 16, 'neigh_op_tnl_6')

wire n3;
// (11, 15, 'neigh_op_tnr_4')
// (11, 16, 'neigh_op_rgt_4')
// (11, 17, 'logic_op_bnr_4')
// (12, 15, 'neigh_op_top_4')
// (12, 16, 'local_g1_4')
// (12, 16, 'lutff_4/out')
// (12, 16, 'lutff_7/in_2')
// (12, 17, 'logic_op_bot_4')
// (13, 15, 'logic_op_tnl_4')
// (13, 16, 'logic_op_lft_4')

wire b;
// (11, 16, 'neigh_op_tnr_0')
// (11, 16, 'neigh_op_tnr_4')
// (12, 16, 'local_g0_0')
// (12, 16, 'local_g1_0')
// (12, 16, 'lutff_4/in_3')
// (12, 16, 'lutff_7/in_1')
// (12, 16, 'neigh_op_top_0')
// (12, 16, 'neigh_op_top_4')
// (12, 17, 'io_0/D_IN_0')
// (12, 17, 'io_0/PAD')

wire a;
// (11, 16, 'neigh_op_tnr_2')
// (11, 16, 'neigh_op_tnr_6')
// (12, 16, 'local_g0_2')
// (12, 16, 'lutff_4/in_2')
// (12, 16, 'neigh_op_top_2')
// (12, 16, 'neigh_op_top_6')
// (12, 17, 'io_1/D_IN_0')
// (12, 17, 'io_1/PAD')

wire n6;
// (12, 16, 'lutff_7/lout')

wire n7;
// (12, 16, 'lutff_4/lout')

assign n6 = /* LUT   12 16  7 */ (s ? b : n3);
assign n7 = /* LUT   12 16  4 */ (b ? a : 1'b0);
/* FF 12 16  7 */ assign y = n6;
/* FF 12 16  4 */ assign n3 = n7;

endmodule

