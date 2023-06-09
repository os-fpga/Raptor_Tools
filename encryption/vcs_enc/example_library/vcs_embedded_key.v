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
`pragma protect version=1
`pragma protect encrypt_agent="VCS"
`pragma protect encrypt_agent_info="S-2021.09-SP2 Build Date Feb 24 2022 20:42:21"
`pragma protect key_keyowner="Synopsys"
`pragma protect key_keyname="SNPS-VCS-RSA-2"
`pragma protect key_method="rsa"
`pragma protect key_public_key
MIGfMA0GCSqGSIb3DQEBAQUAA4GNADCBiQKBgQC7+yvySxqMMiYXyFldRrNrGr0K
mqGSHT3aSSS8o6VZYNxtktRQRqpQsTbVBBCS5zPBnD1ND3Ykbz06nL66GRTr19VZ
MJyxt5FS2e9l2FqxniwVhm0UCQLTNoX3Fy0Zs0igKxVf8FwrfwTDbEscuvmzcNSg
1AXQBqFttqfTIhjHpwIDAQAB
`pragma protect begin
  assign c = 8'hca >> { b, a, 1'h0 };
  `pragma protect end
endmodule