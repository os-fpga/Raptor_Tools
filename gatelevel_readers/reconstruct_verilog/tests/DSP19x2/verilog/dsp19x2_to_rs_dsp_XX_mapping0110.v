//
// Copyright (C) 2023 RapidSilicon
// DSP19x2 Primitive Forward Mapping
//
//

module DSP19X2 #(
  parameter DSP_MODE = "MULTIPLY_ACCUMULATE", // DSP arithmetic mode (MULTIPLY/MULTIPLY_ACCUMULATE)
  parameter [9:0] COEFF1_0 = 10'h000, // Multiplier 1 10-bit A input coefficient 0
  parameter [9:0] COEFF1_1 = 10'h000, // Multiplier 1 10-bit A input coefficient 1
  parameter [9:0] COEFF1_2 = 10'h000, // Multiplier 1 10-bit A input coefficient 2
  parameter [9:0] COEFF1_3 = 10'h000, // Multiplier 1 10-bit A input coefficient 3
  parameter [9:0] COEFF2_0 = 10'h000, // Multiplier 2 10-bit A input coefficient 0
  parameter [9:0] COEFF2_1 = 10'h000, // Multiplier 2 10-bit A input coefficient 1
  parameter [9:0] COEFF2_2 = 10'h000, // Multiplier 2 10-bit A input coefficient 2
  parameter [9:0] COEFF2_3 = 10'h000, // Multiplier 2 10-bit A input coefficient 3
  parameter OUTPUT_REG_EN = "TRUE", // Enable output register (TRUE/FALSE)
  parameter INPUT_REG_EN = "TRUE" // Enable input register (TRUE/FALSE)
) (
  input wire [9:0] A1, // Multiplier 1 10-bit data input for multiplier or accumulator loading
  input wire [8:0] B1, // 9-bit data input for multiplication
  output wire [18:0] Z1, // Multiplier 1 19-bit data output
  output wire [8:0] DLY_B1, // Multiplier 1 9-bit B registered output
  input wire [9:0] A2, // Multiplier 2 10-bit data input for multiplier or accumulator loading
  input wire [8:0] B2, // Multiplier 2 9-bit data input for multiplication
  output wire [18:0] Z2, // Multiplier 2 19-bit data output
  output wire [8:0] DLY_B2, // Multiplier 2 9-bit B registered output
  input wire CLK, // Clock
  input wire RESET, // Reset input
  input wire [4:0] ACC_FIR, // 5-bit left shift A input
  input wire [2:0] FEEDBACK, // 3-bit feedback input selects coefficient
  input wire LOAD_ACC, // Load accumulator input
  input wire UNSIGNED_A, // Selects signed or unsigned data for A input
  input wire UNSIGNED_B, // Selects signed or unsigned data for B input
  input wire SATURATE, // Saturate enable
  input wire [4:0] SHIFT_RIGHT, // 5-bit Shift right
  input wire ROUND, // Round
  input wire SUBTRACT // Add or subtract
);
    localparam  accumulator         = (DSP_MODE == "MULTIPLY_ACCUMULATE") ? 1'h1 : 1'h0;
    localparam  adder               = (DSP_MODE == "MULTIPLY_ADD_SUB") ? 1'h1 : 1'h0;
    localparam  output_reg          = (OUTPUT_REG_EN == "TRUE") ? 1'h1 : 1'h0;
    localparam  input_reg           = (INPUT_REG_EN == "TRUE") ? 1'h1 : 1'h0;
    localparam [0:2] output_select  = {accumulator, adder, output_reg};

        //else if (output_select == 3'b011 && input_reg == 1'b0) begin
            RS_DSP_MULTADD_REGOUT # (
                .MODE_BITS({{COEFF1_0, COEFF2_0}, {COEFF1_1, COEFF2_1}, {COEFF1_2, COEFF2_2}, {COEFF1_3, COEFF2_3}, 4'bxxxx, 1'b1})
            ) RS_DSP_MULTADD_REGOUT (
                .a({A1, A2}), 
                .b({B1, B2}), 
                .z({Z1, Z2}),  
                .dly_b({DLY_B1, DLY_B2}),
                .feedback(FEEDBACK), 
                .unsigned_a(UNSIGNED_A), 
                .unsigned_b(UNSIGNED_B),
                .clk(CLK), 
                .lreset(RESET), 
                .acc_fir(ACC_FIR), 
                .load_acc(LOAD_ACC), 
                .saturate_enable(SATURATE),
                .shift_right(SHIFT_RIGHT), 
                .round(ROUND), 
                .subtract(SUBTRACT)
            );

endmodule
