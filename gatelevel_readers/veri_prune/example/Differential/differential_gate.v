`timescale 1ns/1ps
//  This is an example Gate-level code to instantiate differential inputs/outputs or the post-syntehsis netlist result from differential_rtl 
//  This file can be read into synthesis to have direct implementation without inferrence or is considered an optimal output netlist from
//     differential_rtl.v showing primitve mapping and more optimal naming conventions for wires and instances.
//  This example include attributes to differential_rtl to allow future cross-probing to source.

module differential (
  (* src = "differential_rtl.v:5" *)
  input D_p,
  (* src = "differential_rtl.v:5" *)
  input D_n,
  (* src = "differential_rtl.v:6" *)
  input CLK_p,
  (* src = "differential_rtl.v:6" *)
  input CLK_n,
  (* src = "differential_rtl.v:7" *)
  output Q_p,
  (* src = "differential_rtl.v:7" *)
  output Q_n
);

  (* src = "differential_rtl.v:11" *)
  wire d_wire;
  (* src = "differential_rtl.v:12" *)
  wire clk;
  (* src = "differential_rtl.v:6" *)
  wire CLK_clk_buf;
  (* src = "differential_rtl.v:10" *)
  wire q_reg;

  (* src = "differential_rtl.v:15" *)
  I_BUF_DS #(
    .WEAK_KEEPER("NONE")
  ) D_i_buf_ds_inst (
    .I_P(D_p),
    .I_N(D_n),
	.E(1'b1),
	.O(d_wire));

  (* src = "differential_rtl.v:23" *)
  I_BUF_DS #(
    .WEAK_KEEPER("NONE")
  ) CLK_i_buf_ds_inst (
    .I_P(CLK_p),
    .I_N(CLK_n),
	.E(1'b1),
	.O(clk));
	
  (* src = "differential_rtl.v:12" *)
  CLK_BUF CLK_clk_buf_inst (
    .I(clk),
	.O(clk_clk_buf));

  (* src = "differential_rtl.v:11" *)
  DFFRE q_reg_dff_inst (
    .D(d_wire),
	.R(1'b0),
	.E(1'b1),
	.C(clk_clk_buf),
	.Q(q_reg));

  (* src = "differential_rtl.v:38" *)
  O_BUF_DS #(
    .WEAK_KEEPER("NONE")
  ) Q_o_buf_inst (
    .I(q_reg),
	.O_P(Q_p),
	.O_N(Q_n));

endmodule
