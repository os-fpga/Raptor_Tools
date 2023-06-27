
module intf (a, b, c, a_in, b_in, c_in) ;
    input a ; 
    input b ; 
    output c ; 
    output a_in ; 
    output b_in ; 
    input c_in ; 

    \I_BUF(PULL_UP_DOWN="FALSE",REG_EN="FALSE")  #(.DELAY(1'b0),.PULL_UP_DOWN(7'b0),.REG_EN(7'b0),.SLEW_RATE(6'b0)) a_sig (.I(a), .O(a_in)) ; 
    \I_BUF(PULL_UP_DOWN="FALSE",REG_EN="FALSE")  #(.DELAY(1'b0),.PULL_UP_DOWN(7'b0),.REG_EN(7'b0),.SLEW_RATE(6'b0)) b_sig (.I(b), .O(b_in)) ; 
    \O_BUF(PULL_UP_DOWN="FALSE",REG_EN="FALSE")  #(.DELAY(1'b0),.PULL_UP_DOWN(7'b0),.REG_EN(7'b0),.SLEW_RATE(6'b0)) c_sig (.O(c), .I(c_in)) ; 
endmodule

////////////////////// post route module will be there ////////////////
module adder (a_in, b_in, c_in) ;
    input a_in ; 
    input b_in ; 
    output c_in ; 
    (* src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:7" *) wire a_in ; 
    (* src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:7" *) wire b_in ; 
    (* src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:7" *) wire c_in ; 
    \$lut  #(.LUT(4'b0110),.WIDTH(32'b010)) _0_ (.A({a_in,b_in}), .Y(c_in)) ;  
endmodule
/////////////////////////////////////////////////////////////////////////

module top (a, b, c) ;
    wire a_in ; 
    wire b_in ; 
    wire c_in ; 
    input a ; 
    input b ; 
    output c ; 
    (* src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:2", src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:2" *) wire a ; 
    (* src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:3", src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:3" *) wire b ; 
    (* src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:4", src="/nfs_scratch/zafar/RAPTOR_IP/Raptor_IP/Raptor_IP.srcs/sources_1/adder.sv:4" *) wire c ; 
    intf inst_intf (.a(a), .b(b), .c(c), .a_in(a_in), .b_in(b_in), .c_in(c_in))
    adder inst1 (.a_in(a_in), .b_in(b_in), .c_in(c_in)) ; 
endmodule


