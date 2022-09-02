/* Generated by Yosys 0.14+36 (git sha1 4cd3e3b51, gcc 8.2.0 -fPIC -Os) */

module and2_latch(a, b, clk, c, d);
  input a;
  wire a;
  input b;
  wire b;
  output c;
  wire c;
  input clk;
  wire clk;
  output d;
  wire d;
  assign c = 4'b1000 >> { b, a };
  dff _1_ (
    .C(clk),
    .D(c),
    .Q(d)
  );
endmodule