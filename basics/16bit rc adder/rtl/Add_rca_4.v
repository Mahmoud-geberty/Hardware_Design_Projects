// 4 bit ripple carry adder structural model 
// using full adders.

module Add_rca_4 (
    output [3: 0] sum, output c_out, 
    input  [3: 0] a, b, input c_in
);

    wire c_in2, c_in3, c_in4;

    Add_full  FA1   (sum[0], c_in2, a[0], b[0], c_in);
    Add_full  FA2   (sum[1], c_in3, a[1], b[1], c_in2);
    Add_full  FA3   (sum[2], c_in4, a[2], b[2], c_in3);
    Add_full  FA4   (sum[3], c_out, a[3], b[3], c_in4);

endmodule