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

    // parameter [32767:0] INIT          = {32768{1'b0}}, // Initial Contents of data memory
    // parameter [4095:0]  INIT_PARITY   = {4096{1'b0}}, // Initial Contents of parity memory

function [18432-1:0] sram1();
    integer i;
    sram1 = {18432{1'b0}};
    for (i = 0; i < 2048; i=i+2) begin // When i = 0,2,4,6,8.... (Even) (Total 1024 iterations)
        sram1[(i*9)+:16]             = INIT[(i*16)+:16];      // Extracts first 16-bits from INIT and place into fisrt 16 places in SRAM1.
        sram1[(((i+2)*9)-2)+:2]      = INIT_PARITY[(i*2)+:2]; // Extracts first 2-bits from INIT_PARITY and place into the next two places in SRAM1.
        end
endfunction

function [18432-1:0] sram2();
    integer i;
    sram2 = {18432{1'b0}};
    for (i = 1; i < 2048; i=i+2) begin // When i = 1,3,5,7,9.... (Odd) (Total 1024 iterations)
        sram2[((i-1)*9)+:16]         = INIT[(i*16)+:16];      // Extracts next 16-bits onwards to 16 bits from INIT and place into the first 16 places in SRAM2.
        sram2[(((i+1)*9)-2)+:2]      = INIT_PARITY[(i*2)+:2]; // Extracts next 2-bits onwards to 2-bits from INIT_PARITY and place into the next two places in SRAM2.
        end
endfunction

localparam INIT_i1 = {sram2(), sram1()};

/* Compare the C++ calculated memory imported as param and the newly generated memory content*/

  initial begin
    if (INIT_i1 == INIT_i1_cpp)  begin
      $display("SUCCESS!");
    end
    else begin
      $display("FAILED!");
      $display("INIT_i1     = %b\n", INIT_i1[288-1:0]);
      $display("INIT_i1_cpp = %b\n", INIT_i1_cpp[288-1:0]);
    end
  end
endmodule
