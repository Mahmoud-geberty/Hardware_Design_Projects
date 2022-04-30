// mostly the same design as the transitter. 

module uart_rx #(
    BAUD_RATE = 115200,
    CLK_RATE = 100000000, // 100MHz clock 
    WORD_WIDTH = 8,
    EVEN_PARITY = 0
) (
    input                          clock,
    input                          rst,
    input                          rx_data_in,
    // output                         rx_ready,
    output  [WORD_WIDTH - 1: 0]    rx_data_out,
    output                         rx_data_valid,
    output                         rx_bit_error
    // output  [WORD_WIDTH : 0]       rx_dbg_shiftreg,
    // output  s_idle, s_start, s_data, s_parity, s_stop, s_wait
);


    // state constants
    typedef enum {IDLE, START, DATA, PARITY, STOP, WAIT} state;
    state  current_state = IDLE;
    state  next_state = IDLE;

    // debug outputs
    // assign s_idle = current_state == IDLE;
    // assign s_start = current_state == START;
    // assign s_data = current_state == DATA;
    // assign s_parity = current_state == PARITY;
    // assign s_stop = current_state == STOP;
    // assign s_wait = current_state == WAIT;

    // logic [WORD_WIDTH: 0] shiftreg;
    // assign shiftreg = rx_data_shiftreg;
    // assign rx_dbg_shiftreg = shiftreg;

    // calculate constants to be used further down. 
    localparam BAUD_COUNTER_MAX = CLK_RATE / BAUD_RATE; 
    localparam BAUD_COUNTER_SIZE = $clog2(BAUD_COUNTER_MAX);

    localparam DATA_COUNTER_MAX = WORD_WIDTH;
    localparam DATA_COUNTER_SIZE = $clog2(DATA_COUNTER_MAX);


    // internal signals
    logic                               rx_data_ready_i;
    logic   [WORD_WIDTH - 1: 0]         rx_data_out_i;
    logic   [WORD_WIDTH: 0]             rx_data_buffer_i; // MSB will be the parity bit
    
    logic   [WORD_WIDTH: 0]             rx_data_shiftreg; 
    logic   [DATA_COUNTER_SIZE - 1: 0]  rx_data_counter_i;
    logic   [BAUD_COUNTER_SIZE: 0]      rx_baud_counter_i;

    logic                               rx_parity;
    logic                               parity_bit_i; 
    logic                               w_data;

    logic   rx_baud_done_i;
    logic   rx_baud_mid_i; 
    logic   rx_data_done_i;
    logic   no_baud_rst;

    logic   state_transition; 
    assign  state_transition = current_state != next_state; 

    assign rx_data_ready_i = (current_state == IDLE);
    assign rx_ready = rx_data_ready_i;

    // the baud counter
    always @(posedge clock) begin 
        if (rst) begin
            rx_baud_counter_i <= 'd0;
        end
        else begin 
            if ( rx_baud_done_i || state_transition) begin 
                if(!no_baud_rst) begin 
                    rx_baud_counter_i <= 'd0;
                end
                else begin 
                    rx_baud_counter_i <= rx_baud_counter_i + 'd1; 
                end
            end
            else begin
                rx_baud_counter_i <= rx_baud_counter_i + 'd1;
            end 
        end
    end

    assign rx_baud_done_i = rx_baud_counter_i >= BAUD_COUNTER_MAX - 1;
    assign rx_baud_mid_i = rx_baud_counter_i == ( BAUD_COUNTER_MAX + (BAUD_COUNTER_MAX / 2) ) - 1; 

    // wait an extra half baud cycle during start bit
    // to make sure the bit is read in the middle.
    assign no_baud_rst = (current_state == START && next_state == START); 

    // the rx shift register
    always @(posedge clock) begin 
        if ( rst ) begin 
            rx_data_shiftreg <= 0; 
        end
        else if (rx_baud_done_i || state_transition) begin 
            rx_data_shiftreg <= rx_data_shiftreg >> 1; 
            rx_data_shiftreg[WORD_WIDTH] <= w_data;
        end
    end

    // the data bit counter.
    always @(posedge clock) begin
        if ( rst ) begin 
            rx_data_counter_i <= 'd0;
        end
        else if (rx_baud_done_i) begin 
            if (state_transition) begin 
                rx_data_counter_i <= 'd0;
            end
            else begin 
                rx_data_counter_i <= rx_data_counter_i + 1;
            end 
        end 
    end

    assign rx_data_done_i = rx_data_counter_i == DATA_COUNTER_MAX - 1;

    // state machine (combinatorial)
    always @(*) begin 
        case (current_state)
            IDLE: begin 
                if (!rx_data_in) begin 
                    next_state = START;
                end
                else begin 
                    next_state = current_state;
                end
            end

            START: begin 
                if ( rx_baud_mid_i) begin
                    next_state = DATA;
                end
                else begin 
                    next_state = current_state;
                end
            end

            DATA: begin
                if (rx_baud_done_i && rx_data_done_i) begin 
                    next_state = PARITY;
                end
                else begin 
                    next_state = current_state;
                end
            end

            PARITY: begin 
                if ( rx_baud_done_i) begin 
                    next_state = STOP;
                end
                else begin 
                    next_state = current_state;
                end
            end

            STOP: begin
                if ( rx_baud_done_i) begin 
                    next_state = WAIT;
                end
                else begin 
                    next_state = current_state;
                end
            end

            WAIT: begin 
                if ( rx_baud_done_i) begin 
                    next_state = IDLE;
                end
                else begin 
                    next_state = current_state;
                end
            end

            default: next_state = current_state;
        endcase
    end 

    // state transitions. 
    always @(posedge clock) begin 
        if ( rst) begin 
            current_state <= IDLE;
            rx_data_out_i <= 8'd0;
        end
        else if (current_state == PARITY) begin 
            rx_data_buffer_i <= rx_data_shiftreg;
            current_state <= next_state;
        end
        else if ( current_state == STOP && next_state == WAIT) begin
            { parity_bit_i, rx_data_out_i} <= rx_data_buffer_i; 
            current_state <= next_state;
        end
        else if (next_state == START) begin 
            rx_data_out_i <= 8'd0; 
            current_state <= next_state;
        end
        else begin 
            current_state <= next_state;
        end
    end

    assign rx_data_out = rx_data_out_i;
    assign w_data = rx_data_in;

    assign rx_parity = (EVEN_PARITY)? ^rx_data_out_i: ~^rx_data_out_i;
    assign rx_bit_error = (rx_parity != parity_bit_i);

    assign rx_data_valid = (current_state == WAIT);

endmodule