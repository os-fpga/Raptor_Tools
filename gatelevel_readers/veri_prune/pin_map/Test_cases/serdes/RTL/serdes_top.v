(* blackbox *)
module O_BUFT #(
  parameter WEAK_KEEPER = "NONE" // Enable pull-up/pull-down on output (NONE/PULLUP/PULLDOWN)
`ifdef RAPIDSILICON_INTERNAL
  ,   parameter IOSTANDARD = "DEFAULT", // IO Standard
  parameter DRIVE_STRENGTH = 2, // Drive strength in mA for LVCMOS standards
  parameter SLEW_RATE = "SLOW" // Transition rate for LVCMOS standards
`endif // RAPIDSILICON_INTERNAL
  ) (
  input logic I,
  input logic T,
  output logic O
);
endmodule

 

 

(* blackbox *)
module O_BUF
`ifdef RAPIDSILICON_INTERNAL
  #(
  parameter IOSTANDARD = "DEFAULT", // IO Standard
  parameter DRIVE_STRENGTH = 2, // Drive strength in mA for LVCMOS standards
  parameter SLEW_RATE = "SLOW" // Transition rate for LVCMOS standards
  )
`endif // RAPIDSILICON_INTERNAL
  (
  input logic I,
  output logic O
);
endmodule

 

(* blackbox *)
module I_BUF #(
  parameter WEAK_KEEPER = "NONE" // Specify Pull-up/Pull-down on input (NONE/PULLUP/PULLDOWN)
`ifdef RAPIDSILICON_INTERNAL
  ,   parameter IOSTANDARD = "DEFAULT" // IO Standard
`endif // RAPIDSILICON_INTERNAL
  ) (
  input logic I,
  input logic EN,
  output logic O
);
endmodule

 

(* blackbox *)
module I_SERDES #(
  parameter DATA_RATE = "SDR", // Single or double data rate
  parameter WIDTH = 4, // Width of Deserialization (3-10)
  parameter DPA_MODE = "NONE" // Select Dynamic Phase Alignment or Clock Data Recovery (NONE/DPA/CDR)
  ) (
  input logic D,
  input logic RST,
  input logic FIFO_RST,
  input logic BITSLIP_ADJ,
  input logic EN,
  input logic CLK_IN,
  output logic CLK_OUT,
  output logic [WIDTH-1:0]Q,
  output logic DATA_VALID,
  output logic DPA_LOCK,
  output logic DPA_ERROR,
  input logic PLL_LOCK,
  input logic PLL_FAST_CLK
);
endmodule

 

(* blackbox *)
module O_SERDES #(
  parameter DATA_RATE = "SDR", // Single or double data rate
  parameter WIDTH = 4, // Width of input data to serializer
  parameter CLOCK_PHASE = 0 // Clock phase
  ) (
  input logic [WIDTH-1:0]D,
  input logic RST,
  input logic LOAD_WORD,
  output logic OE,
  input logic CLK_EN,
  input logic CLK_IN,
  output logic CLK_OUT,
  output logic Q,
  input logic CHANNEL_BOND_SYNC_IN,
  output logic CHANNEL_BOND_SYNC_OUT,
  input logic PLL_LOCK,
  input logic PLL_FAST_CLK
);
endmodule

(* blackbox *)
module PLL #(
  parameter DIVIDE_CLK_IN_BY_2 = "FALSE", // Enable input divider (TRUE/FALSE)
  parameter PLL_MULT = 16, // Clock multiplier value (16-1000)
  parameter PLL_DIV = 1, // Clock divider value (1-63)
  parameter CLK_OUT0_DIV = 2, // CLK_OUT0 divider value (2,3,4,5,6,7,8,10,12,16,20.24.32.40,48,64)
  parameter CLK_OUT1_DIV = 2, // CLK_OUT1 divider value (2,3,4,5,6,7,8,10,12,16,20.24.32.40,48,64)
  parameter CLK_OUT2_DIV = 2, // CLK_OUT2 divider value (2,3,4,5,6,7,8,10,12,16,20.24.32.40,48,64)
  parameter CLK_OUT3_DIV = 2 // CLK_OUT3 divider value (2,3,4,5,6,7,8,10,12,16,20.24.32.40,48,64)
) (
  input PLL_EN, // PLL Enable
  input CLK_IN, // Clock input
  input CLK_OUT0_EN, // Enable CLK_OUT0
  input CLK_OUT1_EN, // Enable CLK_OUT1
  input CLK_OUT2_EN, // Enable CLK_OUT2
  input CLK_OUT3_EN, // Enable CLK_OUT3
  output CLK_OUT0, // CLK_OUT0 output
  output CLK_OUT1, // CLK_OUT1 output
  output CLK_OUT2, // CLK_OUT2 output
  output CLK_OUT3, // CLK_OUT3 output
  output GEARBOX_FAST_CLK, // Gearbox fast clock output
  output LOCK // PLL lock signal
);
endmodule

 

module serdes_top #(
    parameter WIDTH = 4
) (
    input clk,
    input rst,
    input D,
    output Q,
    input clk_select
);

 

wire fast_clk, pll_lock, d_buf;
wire [WIDTH - 1: 0] des_data;
reg [WIDTH - 1: 0] data_register, processedData1, processedData_temp, processedData; 
reg loaded;
wire channel_bond_sync_out;
wire dpa_lock_and_dpa_error_out, dpa_lock_and_dpa_error;
wire clk_out1, clk_out2, clk_mux_sel;
wire load_data;
reg fifo_rst;
wire out_en;
wire out_buf;

 

assign clk_mux = (clk_mux_sel) ? clk_out1 : clk_out2;

 

always @(posedge clk_mux or posedge rst_buf) begin
    if (rst_buf) begin
        fifo_rst <= 1'b1;
    end else begin
        fifo_rst <= rst_buf;
    end
end

 

always @(posedge clk_mux) begin
    if (rst_buf) begin
      data_register <= 4'b0;
      loaded <= 1'b0;
    end else if (load_data) begin
      data_register <= des_data;
      loaded <= 1'b1;
    end else begin
      data_register <= data_register;
      loaded <= loaded;
    end
  end

  always @(posedge clk_mux) begin
    if (rst_buf) begin
	    processedData1 = 0; // Left shift by DATA_SIZE
    end else begin
	    processedData1 = processedData_temp + 1; // Left shift by DATA_SIZE
	end
  end

   always @* begin
    // Processing logic goes here
    processedData_temp = data_register << 1; // Add '1 to each data element
    processedData = processedData1 + data_register;
    // Add more operations as needed
  end

 

I_BUF #(
    .WEAK_KEEPER("NONE")
) i_buf_inst (
    .I(clk),
    .EN(1'b1),
    .O(clk_buf)
);

 

 

I_BUF #(
    .WEAK_KEEPER("NONE")
) i_buf_inst0 (
    .I(D),
    .EN(1'b1),
    .O(d_buf)
);

 

I_BUF #(
    .WEAK_KEEPER("NONE")
) i_buf_inst1 (
    .I(rst),
    .EN(1'b1),
    .O(rst_buf)
);

 

I_BUF #(
    .WEAK_KEEPER("NONE")
) i_buf_inst2 (
    .I(clk_select),
    .EN(1'b1),
    .O(clk_mux_sel)
);

 


PLL #(
    .DIVIDE_CLK_IN_BY_2("FALSE"),
    .PLL_MULT(16),
    .PLL_DIV(1),
    .CLK_OUT0_DIV(2),
    .CLK_OUT1_DIV(2),
    .CLK_OUT2_DIV(2),
    .CLK_OUT3_DIV(2)
) pll_inst(
    .PLL_EN(1'b1),
    .CLK_IN(clk_buf),
    .CLK_OUT0_EN(),
    .CLK_OUT1_EN(),
    .CLK_OUT2_EN(),
    .CLK_OUT3_EN(),
    .CLK_OUT0(),
    .CLK_OUT1(),
    .CLK_OUT2(),
    .CLK_OUT3(),
    .GEARBOX_FAST_CLK(fast_clk),
    .LOCK(pll_lock)
);

 

I_SERDES #(
    .DATA_RATE("SDR"),
    .WIDTH(4),
    .DPA_MODE("NONE")
) i_serdes_inst (
    .D(d_buf),
    .RST(rst_buf),
    .FIFO_RST(fifo_rst),
    .BITSLIP_ADJ(1'b0),
    .EN(1'b1),
    .CLK_IN(clk_mux),
    .CLK_OUT(clk_out1),
    .Q(des_data),
    .DATA_VALID(load_data),
    .DPA_LOCK(dpa_lock),
    .DPA_ERROR(dpa_error),
    .PLL_LOCK(pll_lock),
    .PLL_FAST_CLK(fast_clk)
);

 

assign dpa_lock_and_dpa_error = dpa_lock & dpa_error;

 

O_BUF o_buf_inst (
    .I(dpa_lock_and_dpa_error),
    .O(dpa_lock_and_dpa_error_out)
);

 

O_BUFT #(
    .WEAK_KEEPER("NONE")
) o_buft_inst (
    .I(ser_data),
    .T(out_en),
    .O(Q)
);

 

 

O_SERDES #(
    .DATA_RATE("SDR"),
    .WIDTH(4),
    .CLOCK_PHASE(0)
) o_serdes_inst (
    .D(processedData),
    .RST(rst_buf),
    .LOAD_WORD(loaded),
    .OE(out_en),
    .CLK_EN(1'b1),
    .CLK_IN(clk_mux),
    .CLK_OUT(clk_out2),
    .Q(ser_data),
    .CHANNEL_BOND_SYNC_IN(1'b0),
    .CHANNEL_BOND_SYNC_OUT(channel_bond_sync_out),
    .PLL_LOCK(pll_lock),
    .PLL_FAST_CLK(fast_clk)
);

 

 

endmodule 
