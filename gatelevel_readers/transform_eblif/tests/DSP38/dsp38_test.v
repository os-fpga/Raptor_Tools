module dsp38_test (
input [17:0] B, output [37:0] Z,
input [19:0] A);
DSP38 #(
    .COEFF_0(20'd20),
    .COEFF_1(20'd40),
    .COEFF_2(20'd60),
    .COEFF_3(20'd80),
    .DSP_MODE("MULTIPLY_ACCUMULATE"),
    .OUTPUT_REG_EN("FALSE"),
    .INPUT_REG_EN("FALSE")
) DSP38_a (
    .A(A),
    .B(B),
    .ACC_FIR(6'd0),
    .Z(Z),
    .DLY_B(),
    .CLK(),
    .RESET(),
    .FEEDBACK(3'd0),
    .LOAD_ACC(1'b1),
    .SATURATE(1'b0),
    .SHIFT_RIGHT(6'd0),
    .ROUND(1'b0),
    .SUBTRACT(1'b0),
    .UNSIGNED_A(1'b1),
    .UNSIGNED_B(1'b1)
);

DSP38 #(
    .COEFF_0(20'd20),
    .COEFF_1(20'd40),
    .COEFF_2(20'd60),
    .COEFF_3(20'd80),
    .DSP_MODE("MULTIPLY"),
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("FALSE")
) DSP38_b (
    .A(A),
    .B(B),
    .ACC_FIR(6'd0),
    .Z(Z),
    .DLY_B(),
    .CLK(),
    .RESET(),
    .FEEDBACK(3'd0),
    .LOAD_ACC(1'b1),
    .SATURATE(1'b0),
    .SHIFT_RIGHT(6'd0),
    .ROUND(1'b0),
    .SUBTRACT(1'b0),
    .UNSIGNED_A(1'b1),
    .UNSIGNED_B(1'b1)
);
endmodule
