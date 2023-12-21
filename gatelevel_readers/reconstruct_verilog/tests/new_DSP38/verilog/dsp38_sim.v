// Copyright (C) 2023 RapidSilicon
//
//Thursday, November 20, 2023

`default_nettype none
// ---------------------------------------- //
// ----- DSP cells simulation modules ----- //
// --------- Control bits in ports -------- //
// ---------------------------------------- //

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

module RS_DSP_MULT_REGIN (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire       clk,
    input  wire       lreset,

    input  wire [2:0] feedback,
    input  wire       unsigned_a,
    input  wire       unsigned_b
);

DSP38 #(
    .DSP_MODE("MULTIPY"),
    .OUTPUT_REG_EN("FALSE"),
    .INPUT_REG_EN("TRUE")
) DSP38_MULT_REGIN (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset)
);

endmodule

module RS_DSP_MULT_REGOUT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire       clk,
    input  wire       lreset,

    input  wire [2:0] feedback,
    input  wire       unsigned_a,
    input  wire       unsigned_b
);

DSP38 #(
    .DSP_MODE("MULTIPY"),
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("FALSE")
) DSP38_MULT_REGOUT (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset)
);

endmodule

module RS_DSP_MULT_REGIN_REGOUT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire       clk,
    input  wire       lreset,

    input  wire [2:0] feedback,
    input  wire       unsigned_a,
    input  wire       unsigned_b
);

DSP38 #(
    .DSP_MODE("MULTIPY"),
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("TRUE")
) DSP38_MULT_REGIN_REGOUT (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset)
);

endmodule

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

module RS_DSP_MULTADD_REGIN (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

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
    .INPUT_REG_EN("TRUE")
) DSP38_MULTADD_REGIN (
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

module RS_DSP_MULTADD_REGOUT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

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
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("FALSE")
) DSP38_MULTADD_REGOUT (
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

module RS_DSP_MULTADD_REGIN_REGOUT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

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
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("TRUE")
) DSP38_MULTADD_REGIN_REGOUT (
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

module RS_DSP_MULTACC (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

    input  wire        load_acc,
    input  wire [ 2:0] feedback,
    input  wire        unsigned_a,
    input  wire        unsigned_b,

    input  wire        saturate_enable,
    input  wire [ 5:0] shift_right,
    input  wire        round,
    input  wire        subtract
);

DSP38 #(
    .DSP_MODE("MULTIPLY_ACCUMULATE"),
    .OUTPUT_REG_EN("FALSE"),
    .INPUT_REG_EN("FALSE")
) DSP38_MULTACC (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset),
    .LOAD_ACC(load_acc),
    .SATURATE(saturate_enable),
    .SHIFT_RIGHT(shift_right),
    .ROUND(round),
    .SUBTRACT(subtract)
);

endmodule

module RS_DSP_MULTACC_REGIN (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

    input  wire [ 2:0] feedback,
    input  wire        load_acc,
    input  wire        unsigned_a,
    input  wire        unsigned_b,

    input  wire        saturate_enable,
    input  wire [ 5:0] shift_right,
    input  wire        round,
    input  wire        subtract
);

DSP38 #(
    .DSP_MODE("MULTIPLY_ACCUMULATE"),
    .OUTPUT_REG_EN("FALSE"),
    .INPUT_REG_EN("TRUE")
) DSP38_MULTACC_REGIN (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset),
    .LOAD_ACC(load_acc),
    .SATURATE(saturate_enable),
    .SHIFT_RIGHT(shift_right),
    .ROUND(round),
    .SUBTRACT(subtract)
);

endmodule

module RS_DSP_MULTACC_REGOUT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

    input  wire [ 2:0] feedback,
    input  wire        load_acc,
    input  wire        unsigned_a,
    input  wire        unsigned_b,

    input  wire        saturate_enable,
    input  wire [ 5:0] shift_right,
    input  wire        round,
    input  wire        subtract
);

DSP38 #(
    .DSP_MODE("MULTIPLY_ACCUMULATE"),
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("FALSE")
) DSP38_MULTACC_REGOUT (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset),
    .LOAD_ACC(load_acc),
    .SATURATE(saturate_enable),
    .SHIFT_RIGHT(shift_right),
    .ROUND(round),
    .SUBTRACT(subtract)
);

endmodule

module RS_DSP_MULTACC_REGIN_REGOUT (
    input  wire [19:0] a,
    input  wire [17:0] b,
    output wire [37:0] z,

    (* clkbuf_sink *)
    input  wire        clk,
    input  wire        lreset,

    input  wire [ 2:0] feedback,
    input  wire        load_acc,
    input  wire        unsigned_a,
    input  wire        unsigned_b,

    input  wire        saturate_enable,
    input  wire [ 5:0] shift_right,
    input  wire        round,
    input  wire        subtract
);

DSP38 #(
    .DSP_MODE("MULTIPLY_ACCUMULATE"),
    .OUTPUT_REG_EN("TRUE"),
    .INPUT_REG_EN("TRUE")
) DSP38_MULTACC_REGIN_REGOUT (
    .A(a),
    .B(b),
    .Z(z),
    .FEEDBACK(feedback),
    .UNSIGNED_A(unsigned_a),
    .UNSIGNED_B(unsigned_b),
    .CLK(clk),
    .RESET(lreset),
    .LOAD_ACC(load_acc),
    .SATURATE(saturate_enable),
    .SHIFT_RIGHT(shift_right),
    .ROUND(round),
    .SUBTRACT(subtract)
);

endmodule