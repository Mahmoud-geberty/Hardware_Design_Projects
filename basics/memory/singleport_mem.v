/*
    Implementation of a single port block ram

    single port memories are seldom used in FPGAs, the purpose of this 
    design is to make use of bi-directional buses for learning. 
*/

module singleport_mem (
    input i_clk, i_read, i_write,
    input [9:0] i_addr, 

    inout [15:0] io_data
);

    // internal registers
    reg        r_read;
    reg [15:0] r_dout;

    // memory declaration
    reg [15:0] mem [0:1023];

    // read write controller
    always @(posedge i_clk) begin
        if (i_write) begin
            r_read <= 0;
            mem[i_addr] <= io_data;
        end

        r_read <= i_read;
        r_dout <= mem[i_addr];
    end

    assign io_data = r_read? r_dout: 16'bz;
    
endmodule