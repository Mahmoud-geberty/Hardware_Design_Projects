/*
    Implementation of a full dual port block ram.
    each port can perfom read and write to the memory.
    each port has its own clock
*/
module full_dp_mem (
    input [15:0] i_din_a, i_din_b,
    input [9:0] i_address_a, i_address_b,
    input i_wen_a, i_wen_b, i_clk_a, i_clk_b,

    output reg [15:0] o_dout_a, o_dout_b
);

    // the actual memory
    reg [15:0] mem [0:1023];

    // port a
    always @(posedge i_clk_a) begin
        // perform memory read from i_address_a
        // if write is not enabled
        o_dout_a <= mem[i_address_a];

        if (i_wen_a) begin
            // stop the read operation
            o_dout_a <= i_din_a;

            // perform memory write to i_address_a
            mem[i_address_a] <= i_din_a;
        end
    end

    // port b
    always @(posedge i_clk_b) begin
        // perform memory read from i_address_b
        // if write is not enabled
        o_dout_b <= mem[i_address_b];

        if (i_wen_b) begin
            // stop the read operation
            o_dout_b <= i_din_b;

            // perform memory write to i_address_b
            mem[i_address_b] <= i_din_b;
        end
    end
    
endmodule