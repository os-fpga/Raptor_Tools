module full_adder (a,b,cin,sum,cout);

input a,b,cin;
output sum,cout;

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
    assign sum = (a ^ b ^ cin);
    assign cout = ((a & b)|(b & cin)|(cin & a));
`pragma protect end

endmodule 