/* Generated by Yosys 0.17+76 (git sha1 035496b50, gcc 9.1.0 -fPIC -Os) */

(* top =  1  *)
(* src = "./rtl/and2/and2.v:7.1-18.10" *)
module and2(a, b, c);
  (* src = "./rtl/and2/and2.v:12.12-12.13" *)
  input a;
  wire a;
  (* src = "./rtl/and2/and2.v:13.12-13.13" *)
  input b;
  wire b;
  (* src = "./rtl/and2/and2.v:14.13-14.14" *)
  output c;
  wire c;
  \$lut  #(
    .LUT(8'hca),
    .WIDTH(32'h00000003)
  ) _0_ (
    .A({ b, a, 1'h0 }),
    .Y(c)
  );
endmodule
