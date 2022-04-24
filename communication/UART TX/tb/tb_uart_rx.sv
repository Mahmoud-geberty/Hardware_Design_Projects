`timescale 1ns/1ns

module tb_uart_rx (); 
    localparam WORD_WIDTH = 8;

    logic                          clock;
    logic                          rst;
    logic                          rx_ready;
    logic   [WORD_WIDTH - 1: 0]    rx_data_out;
    logic  s_idle, s_start, s_data, s_parity, s_stop, s_wait;

    
    logic                          tx_data_valid;
    logic   [WORD_WIDTH - 1: 0]    tx_data_in;
    logic                          tx_ready;
    logic                          w_data;
    logic   [WORD_WIDTH - 1: 0]    tx_dbg_shiftreg;
    logic   [WORD_WIDTH : 0]       rx_dbg_shiftreg;

    // DUT 
    uart_tx  #(
        .CLK_RATE(100000000), // 100MHz clock
        .BAUD_RATE(115200), 
        .WORD_WIDTH(WORD_WIDTH)
    ) tx (
        .s_idle(), .s_data(), .s_parity(), .s_start(), .s_stop(), .s_wait(),
        .tx_data_out(w_data),
         .*
    );


    // DUT 
    uart_rx  #(
        .CLK_RATE(100000000), // 100MHz clock
        .BAUD_RATE(115200), 
        .WORD_WIDTH(WORD_WIDTH)
    ) DUT (
        .rx_data_in(w_data),
        .rx_dbg_shiftreg(rx_dbg_shiftreg),
        .*
    );

    // generate a 100MHz clock
    always #5 clock <= ~clock; 

    // drive the TX same way as in its own tb, 
    // then just observe the RX outputs. 
    // basically use TX to test RX. 
    initial begin
        clock = 0; 
        rst = 1; 
        tx_data_valid = 0; 
        tx_data_in = 8'd0;

        // test the reset
        #20 rst = 0; 

        @(posedge clock); 
        tx_data_in = 8'd91;
        
        @(posedge clock); 
        tx_data_valid = 1;

        @(posedge clock); 
        tx_data_valid = 0; 

        // wait until the message has been sent
        @(posedge tx_ready);
        @(posedge clock); 
        tx_data_in = 8'd120;

        @(posedge clock); 
        tx_data_valid = 1;

        @(posedge clock); 
        tx_data_valid = 0; 
    end

endmodule