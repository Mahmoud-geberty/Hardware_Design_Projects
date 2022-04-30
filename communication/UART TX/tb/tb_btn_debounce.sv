`timescale 1ns/1ns

module tb_btn_debounce ();

    localparam CLK_RATE = 100000000;
    localparam BTN_DELAY_MAX = CLK_RATE/50;
    localparam BTN_DELAY_SIZE = $clog2(BTN_DELAY_MAX);

    logic  clock;
    logic  rst;
    logic  btn;
    logic  tx_send;
    logic  btn_detect_i;
    logic [BTN_DELAY_SIZE - 1: 0] delay_counter_i;

    btn_debounce  b(.*);


    // clock generator, period: 10ns
    initial begin 
        clock = 0;
        forever begin
            #5 clock = ~clock;
        end
    end

    // initial reset
    initial begin 
        rst = 1; 
        #100 rst = 0; 
    end

    // stimulus
    initial begin 
        btn = 0;
        @(negedge rst); // wait for rst 
        #5 btn = 1; // asynchronous btn press
        #20 btn = 0; 

        @(negedge tx_send); 
        #10 btn = 1; 
        #10 btn = 0; 
    end

endmodule