// a gate-level design of the half-adder

module Add_half (
	output sum, c_out,
	input a, b
);
	and   M1(c_out, a, b);
	xor   M2(sum, a, b);

endmodule 