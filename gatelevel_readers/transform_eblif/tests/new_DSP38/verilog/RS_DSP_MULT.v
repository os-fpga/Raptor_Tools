module RS_DSP_MULT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    input  wire [2:0] feedback,
    input  wire       unsigned_a,
    input  wire       unsigned_b
);

DSP38 #(
    .DSP_MODE("MULTIPY"),
    .OUTPUT_REG_EN("FALSE"),
    .INPUT_REG_EN("FALSE")
) DSP38_MULT (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b)
);

endmodule
