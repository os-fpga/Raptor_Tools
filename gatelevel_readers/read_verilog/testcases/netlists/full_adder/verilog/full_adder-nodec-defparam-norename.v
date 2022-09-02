/* Generated by Yosys 0.17+76 (git sha1 035496b50, gcc 9.1.0 -fPIC -Os) */

(* top =  1  *)
(* src = "./rtl/full_adder/full_adder.v:1.1-10.10" *)
module full_adder(a, b, cin, sum, cout);
  wire \$abc$407$abc$220$abc$212$new_n11__new__new_ ;
  wire \$abc$407$abc$220$abc$212$new_n6__new__new_ ;
  wire \$abc$407$abc$220$abc$212$new_n7__new__new_ ;
  wire \$abc$407$abc$220$abc$212$new_n8__new__new_ ;
  wire \$abc$407$abc$220$abc$212$new_n9__new__new_ ;
  wire \$abc$407$auto$simplemap.cc:333:simplemap_lut$372[1]_new_ ;
  wire \$abc$407$auto$simplemap.cc:333:simplemap_lut$387[1]_new_ ;
  wire \$abc$407$auto$simplemap.cc:333:simplemap_lut$392[1]_new_ ;
  (* src = "./rtl/full_adder/full_adder.v:3.7-3.8" *)
  input a;
  wire a;
  (* src = "./rtl/full_adder/full_adder.v:3.9-3.10" *)
  input b;
  wire b;
  (* src = "./rtl/full_adder/full_adder.v:3.11-3.14" *)
  input cin;
  wire cin;
  (* src = "./rtl/full_adder/full_adder.v:4.12-4.16" *)
  output cout;
  wire cout;
  (* src = "./rtl/full_adder/full_adder.v:4.8-4.11" *)
  output sum;
  wire sum;
  assign \$abc$407$abc$220$abc$212$new_n7__new__new_  = 8'hca >> { b, 1'h1, a };
  assign \$abc$407$auto$simplemap.cc:333:simplemap_lut$372[1]_new_  = 2'h1 >> a;
  assign \$abc$407$abc$220$abc$212$new_n6__new__new_  = 8'hca >> { b, \$abc$407$auto$simplemap.cc:333:simplemap_lut$372[1]_new_ , 1'h1 };
  assign \$abc$407$abc$220$abc$212$new_n8__new__new_  = 8'hca >> { \$abc$407$abc$220$abc$212$new_n7__new__new_ , 1'h1, cin };
  assign \$abc$407$abc$220$abc$212$new_n11__new__new_  = 8'hca >> { \$abc$407$abc$220$abc$212$new_n6__new__new_ , \$abc$407$abc$220$abc$212$new_n9__new__new_ , cin };
  assign sum = 8'hca >> { \$abc$407$abc$220$abc$212$new_n11__new__new_ , \$abc$407$abc$220$abc$212$new_n8__new__new_ , 1'h0 };
  assign \$abc$407$auto$simplemap.cc:333:simplemap_lut$387[1]_new_  = 2'h1 >> \$abc$407$abc$220$abc$212$new_n6__new__new_ ;
  assign \$abc$407$abc$220$abc$212$new_n9__new__new_  = 8'hca >> { \$abc$407$abc$220$abc$212$new_n7__new__new_ , \$abc$407$auto$simplemap.cc:333:simplemap_lut$392[1]_new_ , 1'h1 };
  assign cout = 8'hca >> { \$abc$407$abc$220$abc$212$new_n9__new__new_ , \$abc$407$auto$simplemap.cc:333:simplemap_lut$387[1]_new_ , 1'h1 };
  assign \$abc$407$auto$simplemap.cc:333:simplemap_lut$392[1]_new_  = 2'h1 >> cin;
endmodule
