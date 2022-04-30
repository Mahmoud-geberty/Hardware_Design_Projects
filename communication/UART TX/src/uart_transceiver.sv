// A wrapper module to test both UART TX and RX on actual hardware.

// last action: attempt to run tx directly on hardware.
//              nothing happened. 

// TODO:   make a testbench and double-check everything.

module uart_transceiver #(
    CLK_RATE = 100000000,
    BAUD_RATE = 115200, 
    WORD_WIDTH = 8,
    EVEN_PARITY = 0
) (
    input                          clock,
    input                          rst, 
    input                          tx_btn, 
    input                          rx, // the RX pin on the board

    output                         tx, // the tx pin on the board
    output   [WORD_WIDTH - 1: 0]   leds

);

    // specs rundown.
    /*
        1. make data available either as internal registers or using switches,
           then press a button to transmit the character.

        2. visualize the received data using the leds.
    */

    // internal signals
    logic                         tx_send;
    logic    [7:0]                led_i; 

    // continous assignments
    assign   tx_data_in = 8'd90;
    assign   leds       = led_i;

    // bytes to be transmitted 
    logic    [WORD_WIDTH - 1: 0]  tx_data_in;

    // assert if parity error detected at rx
    logic                         rx_bit_error;

    // rx parallel output.
    logic    [WORD_WIDTH - 1: 0]  rx_data_out;

    // rx data buffer. 
    logic    [WORD_WIDTH - 1: 0]  rx_buffer;
    logic    [WORD_WIDTH - 1: 0]  rx_data_valid;

    // buffer the rx parallel output when it is valid.
    always @(posedge clock, posedge rst) begin 
        if ( rst ) begin 
            rx_buffer <= 'd0; 
        end
        else if ( rx_data_valid ) begin 
            rx_buffer <= rx_data_out;
        end 
    end

    // show the last received byte in binary on leds.
    always @(posedge clock, posedge rst) begin 
        if (rst) begin 
            led_i <= 'd0; 
        end
        else begin 
            led_i <= rx_buffer;
        end
    end

    // initialize btn debounce module
    btn_debounce  b (
        .btn(tx_btn), 
        .*
    );

    // initialize the TX nd RX
    uart_tx TX (
        .tx_data_valid(tx_send),
        .tx_data_out(tx),
        .*
    );

    uart_rx Rx (
        .rx_data_in(rx),
        .*
    ); 

    // // toggle the leds to show the design is working.
    // localparam LED_COUNTER_MAX = 100000000/2 ;
    // localparam LED_COUNTER_SIZE = $clog2(LED_COUNTER_MAX);

    // logic [LED_COUNTER_SIZE - 1: 0] led_counter;

    // always @(posedge clock, posedge rst) begin 
    //     if ( rst || led_sw ) begin 
    //         led_counter <= 'd0;
    //     end 
    //     else begin 
    //         led_counter <= led_counter + 'd1;
    //     end
    // end
    // logic led_sw; 
    // assign led_sw = (led_counter == LED_COUNTER_MAX - 1);

    
endmodule