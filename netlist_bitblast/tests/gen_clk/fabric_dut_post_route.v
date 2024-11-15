//Verilog generated by VPR  from post-place-and-route implementation
module fabric_dut (
    input \$fclk_buf_half_clk ,
    input \clk_design ,
    input \data_design ,
    input \enable_design ,
    output \$auto_537 ,
    output \$auto_538 ,
    output \$auto_539 ,
    output \$auto_540 ,
    output \$auto_541 ,
    output \$auto_542 ,
    output \data_half_o_design ,
    output \data_o_design ,
    output \half_clk 
);

    //Wires
    wire \$fclk_buf_half_clk_output_0_0 ;
    wire \clk_design_output_0_0 ;
    wire \data_design_output_0_0 ;
    wire \enable_design_output_0_0 ;
    wire \lut_$auto_537_output_0_0 ;
    wire \lut_$auto_538_output_0_0 ;
    wire \lut_$auto_539_output_0_0 ;
    wire \lut_$auto_540_output_0_0 ;
    wire \lut_$auto_541_output_0_0 ;
    wire \lut_$auto_542_output_0_0 ;
    wire \dffre_data_half_o_design_output_0_0 ;
    wire \dffre_data_o_design_output_0_0 ;
    wire \dffre_half_clk_output_0_0 ;
    wire \lut_$true_output_0_0 ;
    wire \lut_$abc$218$li0_li0_output_0_0 ;
    wire \dffre_data_half_o_design_clock_0_0 ;
    wire \dffre_data_o_design_clock_0_0 ;
    wire \dffre_half_clk_clock_0_0 ;
    wire \dffre_data_o_design_input_0_0 ;
    wire \dffre_data_half_o_design_input_0_0 ;
    wire \dffre_data_o_design_input_2_0 ;
    wire \dffre_data_half_o_design_input_2_0 ;
    wire \$auto_537_input_0_0 ;
    wire \$auto_538_input_0_0 ;
    wire \$auto_539_input_0_0 ;
    wire \$auto_540_input_0_0 ;
    wire \$auto_541_input_0_0 ;
    wire \$auto_542_input_0_0 ;
    wire \data_half_o_design_input_0_0 ;
    wire \data_o_design_input_0_0 ;
    wire \lut_$abc$218$li0_li0_input_0_0 ;
    wire \half_clk_input_0_0 ;
    wire \lut_$auto_539_input_0_3 ;
    wire \lut_$auto_542_input_0_3 ;
    wire \lut_$auto_541_input_0_3 ;
    wire \lut_$auto_538_input_0_3 ;
    wire \dffre_data_o_design_input_1_0 ;
    wire \lut_$auto_537_input_0_1 ;
    wire \lut_$auto_540_input_0_1 ;
    wire \dffre_half_clk_input_1_0 ;
    wire \dffre_half_clk_input_2_0 ;
    wire \dffre_data_half_o_design_input_1_0 ;
    wire \dffre_half_clk_input_0_0 ;

    //IO assignments
    assign \$auto_537  = \$auto_537_input_0_0 ;
    assign \$auto_538  = \$auto_538_input_0_0 ;
    assign \$auto_539  = \$auto_539_input_0_0 ;
    assign \$auto_540  = \$auto_540_input_0_0 ;
    assign \$auto_541  = \$auto_541_input_0_0 ;
    assign \$auto_542  = \$auto_542_input_0_0 ;
    assign \data_half_o_design  = \data_half_o_design_input_0_0 ;
    assign \data_o_design  = \data_o_design_input_0_0 ;
    assign \half_clk  = \half_clk_input_0_0 ;
    assign \$fclk_buf_half_clk_output_0_0  = \$fclk_buf_half_clk ;
    assign \clk_design_output_0_0  = \clk_design ;
    assign \data_design_output_0_0  = \data_design ;
    assign \enable_design_output_0_0  = \enable_design ;

    //Interconnect
    fpga_interconnect \routing_segment_$fclk_buf_half_clk_output_0_0_to_dffre_data_half_o_design_clock_0_0  (
        .datain(\$fclk_buf_half_clk_output_0_0 ),
        .dataout(\dffre_data_half_o_design_clock_0_0 )
    );

    fpga_interconnect \routing_segment_clk_design_output_0_0_to_dffre_data_o_design_clock_0_0  (
        .datain(\clk_design_output_0_0 ),
        .dataout(\dffre_data_o_design_clock_0_0 )
    );

    fpga_interconnect \routing_segment_clk_design_output_0_0_to_dffre_half_clk_clock_0_0  (
        .datain(\clk_design_output_0_0 ),
        .dataout(\dffre_half_clk_clock_0_0 )
    );

    fpga_interconnect \routing_segment_data_design_output_0_0_to_dffre_data_o_design_input_0_0  (
        .datain(\data_design_output_0_0 ),
        .dataout(\dffre_data_o_design_input_0_0 )
    );

    fpga_interconnect \routing_segment_data_design_output_0_0_to_dffre_data_half_o_design_input_0_0  (
        .datain(\data_design_output_0_0 ),
        .dataout(\dffre_data_half_o_design_input_0_0 )
    );

    fpga_interconnect \routing_segment_enable_design_output_0_0_to_dffre_data_o_design_input_2_0  (
        .datain(\enable_design_output_0_0 ),
        .dataout(\dffre_data_o_design_input_2_0 )
    );

    fpga_interconnect \routing_segment_enable_design_output_0_0_to_dffre_data_half_o_design_input_2_0  (
        .datain(\enable_design_output_0_0 ),
        .dataout(\dffre_data_half_o_design_input_2_0 )
    );

    fpga_interconnect \routing_segment_lut_$auto_537_output_0_0_to_$auto_537_input_0_0  (
        .datain(\lut_$auto_537_output_0_0 ),
        .dataout(\$auto_537_input_0_0 )
    );

    fpga_interconnect \routing_segment_lut_$auto_538_output_0_0_to_$auto_538_input_0_0  (
        .datain(\lut_$auto_538_output_0_0 ),
        .dataout(\$auto_538_input_0_0 )
    );

    fpga_interconnect \routing_segment_lut_$auto_539_output_0_0_to_$auto_539_input_0_0  (
        .datain(\lut_$auto_539_output_0_0 ),
        .dataout(\$auto_539_input_0_0 )
    );

    fpga_interconnect \routing_segment_lut_$auto_540_output_0_0_to_$auto_540_input_0_0  (
        .datain(\lut_$auto_540_output_0_0 ),
        .dataout(\$auto_540_input_0_0 )
    );

    fpga_interconnect \routing_segment_lut_$auto_541_output_0_0_to_$auto_541_input_0_0  (
        .datain(\lut_$auto_541_output_0_0 ),
        .dataout(\$auto_541_input_0_0 )
    );

    fpga_interconnect \routing_segment_lut_$auto_542_output_0_0_to_$auto_542_input_0_0  (
        .datain(\lut_$auto_542_output_0_0 ),
        .dataout(\$auto_542_input_0_0 )
    );

    fpga_interconnect \routing_segment_dffre_data_half_o_design_output_0_0_to_data_half_o_design_input_0_0  (
        .datain(\dffre_data_half_o_design_output_0_0 ),
        .dataout(\data_half_o_design_input_0_0 )
    );

    fpga_interconnect \routing_segment_dffre_data_o_design_output_0_0_to_data_o_design_input_0_0  (
        .datain(\dffre_data_o_design_output_0_0 ),
        .dataout(\data_o_design_input_0_0 )
    );

    fpga_interconnect \routing_segment_dffre_half_clk_output_0_0_to_lut_$abc$218$li0_li0_input_0_0  (
        .datain(\dffre_half_clk_output_0_0 ),
        .dataout(\lut_$abc$218$li0_li0_input_0_0 )
    );

    fpga_interconnect \routing_segment_dffre_half_clk_output_0_0_to_half_clk_input_0_0  (
        .datain(\dffre_half_clk_output_0_0 ),
        .dataout(\half_clk_input_0_0 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_lut_$auto_539_input_0_3  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\lut_$auto_539_input_0_3 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_lut_$auto_542_input_0_3  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\lut_$auto_542_input_0_3 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_lut_$auto_541_input_0_3  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\lut_$auto_541_input_0_3 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_lut_$auto_538_input_0_3  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\lut_$auto_538_input_0_3 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_dffre_data_o_design_input_1_0  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\dffre_data_o_design_input_1_0 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_lut_$auto_537_input_0_1  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\lut_$auto_537_input_0_1 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_lut_$auto_540_input_0_1  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\lut_$auto_540_input_0_1 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_dffre_half_clk_input_1_0  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\dffre_half_clk_input_1_0 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_dffre_half_clk_input_2_0  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\dffre_half_clk_input_2_0 )
    );

    fpga_interconnect \routing_segment_lut_$true_output_0_0_to_dffre_data_half_o_design_input_1_0  (
        .datain(\lut_$true_output_0_0 ),
        .dataout(\dffre_data_half_o_design_input_1_0 )
    );

    fpga_interconnect \routing_segment_lut_$abc$218$li0_li0_output_0_0_to_dffre_half_clk_input_0_0  (
        .datain(\lut_$abc$218$li0_li0_output_0_0 ),
        .dataout(\dffre_half_clk_input_0_0 )
    );


    //Cell instances
    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000100000000)
    ) \lut_$auto_539  (
        .in({
            1'b0,
            \lut_$auto_539_input_0_3 ,
            1'b0,
            1'b0,
            1'b0
         }),
        .out(\lut_$auto_539_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000100000000)
    ) \lut_$auto_542  (
        .in({
            1'b0,
            \lut_$auto_542_input_0_3 ,
            1'b0,
            1'b0,
            1'b0
         }),
        .out(\lut_$auto_542_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000100000000)
    ) \lut_$auto_541  (
        .in({
            1'b0,
            \lut_$auto_541_input_0_3 ,
            1'b0,
            1'b0,
            1'b0
         }),
        .out(\lut_$auto_541_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000100000000)
    ) \lut_$auto_538  (
        .in({
            1'b0,
            \lut_$auto_538_input_0_3 ,
            1'b0,
            1'b0,
            1'b0
         }),
        .out(\lut_$auto_538_output_0_0 )
    );

    DFFRE #(
    ) \dffre_data_o_design  (
        .C(\dffre_data_o_design_clock_0_0 ),
        .D(\dffre_data_o_design_input_0_0 ),
        .E(\dffre_data_o_design_input_2_0 ),
        .R(\dffre_data_o_design_input_1_0 ),
        .Q(\dffre_data_o_design_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000000000100)
    ) \lut_$auto_537  (
        .in({
            1'b0,
            1'b0,
            1'b0,
            \lut_$auto_537_input_0_1 ,
            1'b0
         }),
        .out(\lut_$auto_537_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000000000100)
    ) \lut_$auto_540  (
        .in({
            1'b0,
            1'b0,
            1'b0,
            \lut_$auto_540_input_0_1 ,
            1'b0
         }),
        .out(\lut_$auto_540_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000000000001)
    ) \lut_$true  (
        .in({
            1'b0,
            1'b0,
            1'b0,
            1'b0,
            1'b0
         }),
        .out(\lut_$true_output_0_0 )
    );

    LUT_K #(
        .K(5),
        .LUT_MASK(32'b00000000000000000000000000000001)
    ) \lut_$abc$218$li0_li0  (
        .in({
            1'b0,
            1'b0,
            1'b0,
            1'b0,
            \lut_$abc$218$li0_li0_input_0_0 
         }),
        .out(\lut_$abc$218$li0_li0_output_0_0 )
    );

    DFFRE #(
    ) \dffre_half_clk  (
        .C(\dffre_half_clk_clock_0_0 ),
        .D(\dffre_half_clk_input_0_0 ),
        .E(\dffre_half_clk_input_2_0 ),
        .R(\dffre_half_clk_input_1_0 ),
        .Q(\dffre_half_clk_output_0_0 )
    );

    DFFRE #(
    ) \dffre_data_half_o_design  (
        .C(\dffre_data_half_o_design_clock_0_0 ),
        .D(\dffre_data_half_o_design_input_0_0 ),
        .E(\dffre_data_half_o_design_input_2_0 ),
        .R(\dffre_data_half_o_design_input_1_0 ),
        .Q(\dffre_data_half_o_design_output_0_0 )
    );


endmodule
