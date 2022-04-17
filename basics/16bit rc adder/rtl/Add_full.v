// full adder structure model using half adders

module Add_full (
    output sum, c_out,
    input A, B, C_in
);
    wire w1, w2, w3;

    Add_half half(w1, w2, A, B);
    Add_half half2(sum, w3, w1, C_in);

    or       M1(c_out, w2, w3);
endmodule