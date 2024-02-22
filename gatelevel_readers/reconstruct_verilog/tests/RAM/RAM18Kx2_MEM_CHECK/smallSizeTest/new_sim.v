/**
 * @file new_sim.v
 * @author Manadher Kharroubi (manadher@gmail.com)
 * @brief
 * @version 0.1
 * @date 2024-02-20
 *
 * @copyright Copyright (c) 2024
 *
 */
module test();

#include "memData.txt"

function [144-1:0] sram1();
    integer i;
    sram1 = {144{1'b0}};
    for (i = 0; i < 8; i=i+1) begin // (1024*18) = 18432 bits
        sram1[(i*18) +:16]               = INIT1[(i*16) +:16];       // Extracts first 16-bits from INIT1 and place into fisrt 16 places in SRAM1.
        sram1[((i+1)*16+(2*i))+:2]       = INIT1_PARITY[(i*2) +:2];  // Extracts first 2-bits from INIT1_PARITY and place into the next two places in SRAM1.
        end
endfunction

function [144-1:0] sram2();
    integer i;
    sram2 = {144{1'b0}};
    for (i = 0; i < 8; i=i+1) begin // (1024*18) = 18432 bits
        sram2[(i*18) +:16]                = INIT2[(i*16) +:16];      // Extracts first 16-bits from INIT2 and place into fisrt 16 places in SRAM2..
        sram2[((i+1)*16+(2*i)) +:2]      = INIT2_PARITY[(i*2) +:2]; // Extracts first 2-bits from INIT2_PARITY and place into the next two places in SRAM2.
        end
endfunction

localparam [288:0] INIT_i1 = {sram2(), sram1()}; 

initial begin
if (INIT_i1 == INIT_i1_cpp)  begin
      $display("SUCCESS!\n");
end
else begin
      $display("FAILED!\n");
      $display("INIT_i1     = %b\n", INIT_i1[288-1:0]);
      $display("INIT_i1_cpp = %b\n", INIT_i1_cpp[288-1:0]);
end
end
   
endmodule
