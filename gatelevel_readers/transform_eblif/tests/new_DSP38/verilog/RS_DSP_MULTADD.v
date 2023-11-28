
module RS_DSP_MULTADD (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    input  wire       clk,
    input  wire       lreset,

    input  wire [ 2:0] feedback,
    input  wire [ 5:0] acc_fir,
    input  wire        load_acc,
    input  wire        unsigned_a,
    input  wire        unsigned_b,

    input  wire        saturate_enable,
    input  wire [ 5:0] shift_right,
    input  wire        round,
    input  wire        subtract
);

DSP38 #(
    .DSP_MODE("MULTIPLY_ADD_SUB"),
    .OUTPUT_REG_EN("FALSE"),
    .INPUT_REG_EN("FALSE")
) DSP38_MULTADD (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset),
    .ACC_FIR(acc_fir),
    .LOAD_ACC(load_acc),
    .SATURATE(saturate_enable),
    .SHIFT_RIGHT(shift_right),
    .ROUND(round),
    .SUBTRACT(subtract)
);

endmodule
