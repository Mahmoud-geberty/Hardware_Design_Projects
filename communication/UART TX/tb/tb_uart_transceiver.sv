`timescale 1ns/1ns

module tb_uart_transceiver();

    logic                          clock;
    logic                          rst;
    logic                          tx_btn;
    logic                          rx; // the RX pin on the board

    logic                          tx; // the tx pin on the board
    logic  [8 - 1: 0]              leds;

    // DUT 
    uart_transceiver  DUT (
        .*
    );

    // clock generator 
    initial begin 
        clock = 0; 
        forever #5 clock = ~clock;
    end

    // rst to initialize all regester like the fpga does. 
    initial begin 
        rst = 1; 
        #100 rst = 0; 
    end

    // stimulus 
    initial begin
        @(negedge rst); // wait for rst to pull down. 
        #10 tx_btn = 1; 
        #100 tx_btn = 0; 
    end

endmodule