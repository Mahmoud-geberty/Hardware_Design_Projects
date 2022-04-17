// 16 bit ripple carry adder structural model 
// with 4x 4 bit ripple carry adders

module Add_rca_16 (
    output [15: 0] sum, output c_out,
    input  [15: 0] a, b, input c_in
);

    wire w_c2, w_c3, w_c4;

    Add_rca_4  RCA1  (sum[3:0], w_c2, a[3:0], b[3:0], c_in);
    Add_rca_4  RCA2  (sum[7:4], w_c3, a[7:4], b[7:4], w_c2);
    Add_rca_4  RCA3  (sum[11:8], w_c4, a[11:8], b[11:8], w_c3);
    Add_rca_4  RCA4  (sum[15:12], c_out, a[15:12], b[15:12], w_c4);

endmodule