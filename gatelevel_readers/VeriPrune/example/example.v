// Comments before module
module test (a, b, c, d, e, f, v, w, x, y, z) ; // Non-ANSI port list
    // Direction of the ports
    input a, b, c, d, e, f ;
    output v, w, x, y, z ;
    // Net-reg specification of the ports
    wire v, w, x ;
    reg y ;
    reg [3:0] z ;
    
    // An assign statement
    assign w = a + b ;

    // An always block
    always @(c)
    begin
        // A case statement : a parallel_case comment will be added after this line
        case (c)
            0: z = 1 ;
            1: z = 2 ;
            default: z = 0 ;
        endcase
    end

    always@(d)
    begin
        // A case statement : a parallel_case comment will be added after this line
        case(d)
            4'b0111: y = a ;
            4'b1001: y = b ;
            4'b0110: y = 1'b1 ;
            4'b0010: y = 1'b0 ;
            default: y = 1'bx ;
        endcase
    end

    // An assign statement : last module item, will be removed
    assign v = a & e,
           x = b ^ ~f ;

endmodule
// Comments after endmodule

