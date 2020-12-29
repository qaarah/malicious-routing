// Reading file 'example.asc'..

module chip (output y, input b, input a);

wire n1;
// (9, 17, 'span4_horz_r_2')
// (10, 17, 'span4_horz_r_6')
// (11, 16, 'sp4_h_r_0')
// (11, 17, 'span4_horz_r_10')
// (12, 16, 'local_g0_5')
// (12, 16, 'lutff_2/in_3')
// (12, 16, 'sp4_h_r_13')
// (12, 17, 'span4_horz_r_14')
// (13, 16, 'span4_horz_13')
// (13, 16, 'span4_vert_t_14')

wire y;
// (11, 13, 'sp4_r_v_b_41')
// (11, 14, 'sp4_r_v_b_28')
// (11, 15, 'neigh_op_tnr_2')
// (11, 15, 'sp4_r_v_b_17')
// (11, 16, 'neigh_op_rgt_2')
// (11, 16, 'sp4_r_v_b_4')
// (11, 17, 'logic_op_bnr_2')
// (12, 12, 'sp4_h_r_9')
// (12, 12, 'sp4_v_t_41')
// (12, 13, 'sp4_v_b_41')
// (12, 14, 'sp4_v_b_28')
// (12, 15, 'neigh_op_top_2')
// (12, 15, 'sp4_v_b_17')
// (12, 16, 'lutff_2/out')
// (12, 16, 'sp4_v_b_4')
// (12, 17, 'logic_op_bot_2')
// (13, 12, 'io_1/D_OUT_0')
// (13, 12, 'io_1/PAD')
// (13, 12, 'local_g0_1')
// (13, 12, 'span4_horz_9')
// (13, 15, 'logic_op_tnl_2')
// (13, 16, 'logic_op_lft_2')

wire n3;
// (11, 15, 'neigh_op_tnr_5')
// (11, 16, 'neigh_op_rgt_5')
// (11, 17, 'logic_op_bnr_5')
// (12, 15, 'neigh_op_top_5')
// (12, 16, 'local_g1_5')
// (12, 16, 'lutff_2/in_2')
// (12, 16, 'lutff_5/out')
// (12, 17, 'logic_op_bot_5')
// (13, 15, 'logic_op_tnl_5')
// (13, 16, 'logic_op_lft_5')

wire b;
// (11, 16, 'neigh_op_tnr_0')
// (11, 16, 'neigh_op_tnr_4')
// (12, 16, 'local_g0_4')
// (12, 16, 'local_g1_0')
// (12, 16, 'lutff_2/in_1')
// (12, 16, 'lutff_5/in_3')
// (12, 16, 'neigh_op_top_0')
// (12, 16, 'neigh_op_top_4')
// (12, 17, 'io_0/D_IN_0')
// (12, 17, 'io_0/PAD')

wire a;
// (11, 16, 'neigh_op_tnr_2')
// (11, 16, 'neigh_op_tnr_6')
// (12, 16, 'local_g1_2')
// (12, 16, 'lutff_5/in_2')
// (12, 16, 'neigh_op_top_2')
// (12, 16, 'neigh_op_top_6')
// (12, 17, 'io_1/D_IN_0')
// (12, 17, 'io_1/PAD')

wire n6;
// (12, 16, 'lutff_5/lout')

wire n7;
// (12, 16, 'lutff_2/lout')

assign n6 = /* LUT   12 16  5 */ (b ? a : 1'b0);
assign n7 = /* LUT   12 16  2 */ (n1 ? b : n3);
/* FF 12 16  5 */ assign n3 = n6;
/* FF 12 16  2 */ assign y = n7;

endmodule

