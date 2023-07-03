`timescale 1ns/1ps
//  This is an example RTL code using differential I/O

module differential (
  input D_p, D_n,
  input CLK_p, CLK_n,
  output Q_p, Q_n
);

  reg q_reg = 1'b0;
  wire d_wire;
  wire clk;
  
  // Differential buffers must always be instantiated by the user
  I_BUF_DS #(
    .WEAK_KEEPER("NONE")
  ) D_i_buf_ds_inst (
    .I_P(D_p),
    .I_N(D_n),
	.E(1'b1),
	.O(d_wire));

  I_BUF_DS #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_ds_inst (
    .I_P(CLK_p),
    .I_N(CLK_n),
	.E(1'b1),
	.O(clk));
  
  O_BUF_DS #(
    .WEAK_KEEPER("NONE")
  ) Q_o_buf_inst (
    .I(q_reg),
	.O_P(Q_p),
	.O_N(Q_n));

  always @(posedge clk)
    q_reg <= d_wire;
	
endmodule