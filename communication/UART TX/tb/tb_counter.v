`timescale 1ns/1ns
module tb_counter (); 

    reg  T, clk, rst;
    wire tr, tg_main, tg_small, ty;
    wire [4:0] count; 

    parameter PERIOD = 10;

    // DUT 
    counter DUT (
        .T(T), .clk(clk), .rst(rst), 
        .tr(tr), .tg_main(tg_main), .tg_small(tg_small), .ty(ty), 
        .count(count)
    );

    // generate the clock
    always #(PERIOD/2) clk = ~clk;

    initial begin
        T = 0; 
        clk = 0; 
        rst = 1; 

        #15 rst = 0; 

        #20 T = 1; 
        #10 T = 0; 

        #30 rst = 1; 
        #5  rst = 0; 

        @(posedge clk);
        T = 1; 
        #10  T = 0; 

        #80 T = 1; 
        #10 T = 0; 

    end

endmodule