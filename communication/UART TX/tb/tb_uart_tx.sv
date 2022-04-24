`timescale 1ns/1ns

module tb_uart_tx ();

    localparam WORD_WIDTH = 8;

    
    logic                          clock;
    logic                          rst;
    logic                          tx_data_valid;
    logic   [WORD_WIDTH - 1: 0]    tx_data_in;
    logic                          tx_ready;
    logic                         tx_data_out;
    logic  [WORD_WIDTH - 1: 0]    dbg_shiftreg;
    logic  s_idle, s_start, s_data, s_parity, s_stop, s_wait;

    // DUT 
    uart_tx  #(
        .CLK_RATE(100000000), // 100MHz clock
        .BAUD_RATE(115200), 
        .WORD_WIDTH(WORD_WIDTH), 
    ) DUT (
         .*
    );

    // generate clock
    always #5 clock <= ~clock; // 10nm period is 100MHz

    // drive the DUT
    initial begin // initial values
        clock = 0; 
        rst = 0; 
        tx_data_valid = 0; 
        tx_data_in = 8'd0;

        // test the reset
        #20 rst = 1; 
        #5  rst = 0; 

        #5 tx_data_in = 8'd90;
        #5 tx_data_valid = 1;
        #5 tx_data_valid = 0; 

        // wait until the message has been sent
        wait(tx_ready);
        tx_data_in = 8'd120;
        tx_data_valid = 1;
        #5 tx_data_valid = 0; 


    end

endmodule