/*
    Module simple_dp_mem.

    implementation of a simple dual port 1K x 16 block ram. for the purpose
    of seeing how the synthesis tool treats it. it is expected that 
    a block ram instance will be inferred.

*/

module simple_dp_mem (
    input i_clk, i_w_en,
    input [9:0] i_w_addr,
    input [15:0] i_d_in,
    input [9:0] i_r_addr,
    
    output reg [15:0] o_d_out
);

    // initialize a 1000-word register
    // this is the actual memory
    reg [15:0] r_data[0:1023];

    //data read register
    always @(posedge  i_clk) begin
        o_d_out <= r_data[i_r_addr];
    end

    //data write 
    always @(posedge i_clk) begin
        if (i_w_en) begin
            r_data[i_w_addr] <= i_d_in;
        end
    end

endmodule