module uart_tx #(
    BAUD_RATE = 115200,
    CLK_RATE = 100000000, // 100MHz clock 
    WORD_WIDTH = 8,
    EVEN_PARITY = 0
) (
    input                          clock,
    input                          rst,
    input                          tx_data_valid,
    input   [WORD_WIDTH - 1: 0]    tx_data_in,
    output                         tx_ready,
    output                         tx_data_out,
    output  [WORD_WIDTH - 1: 0]    tx_dbg_shiftreg,
    output  s_idle, s_start, s_data, s_parity, s_stop, s_wait
);


    // state constants
    typedef enum {IDLE, START, DATA, PARITY, STOP, WAIT} state;
    state  current_state = IDLE;
    state  next_state = IDLE;

    // debug outputs
    assign s_idle = current_state == IDLE;
    assign s_start = current_state == START;
    assign s_data = current_state == DATA;
    assign s_parity = current_state == PARITY;
    assign s_stop = current_state == STOP;
    assign s_wait = current_state == WAIT;

    // constants for further down usage
    localparam TX_IDLE = 1'b1;
    localparam TX_START = 1'b0;
    localparam TX_WAIT = 1'b1;

    // calculate constants to be used further down. 
    localparam BAUD_COUNTER_MAX = CLK_RATE / BAUD_RATE;
    localparam BAUD_COUNTER_SIZE = $clog2(BAUD_COUNTER_MAX);

    localparam DATA_COUNTER_MAX = WORD_WIDTH;
    localparam DATA_COUNTER_SIZE = $clog2(DATA_COUNTER_MAX);


    // internal signals
    logic                               tx_data_ready_i;
    logic                               tx_data_out_i;
    logic   [WORD_WIDTH - 1: 0]         tx_data_buffer_i;
    logic   [WORD_WIDTH - 1: 0]         tx_data_shiftreg;
    logic   [DATA_COUNTER_SIZE - 1: 0]  tx_data_counter_i;
    logic   [BAUD_COUNTER_SIZE - 1: 0]  tx_baud_counter_i;

    // debug output
    logic [WORD_WIDTH - 1: 0] shiftreg;
    assign shiftreg = tx_data_shiftreg;
    assign tx_dbg_shiftreg = shiftreg;

    logic   tx_baud_done_i;
    logic   tx_data_done_i;

    // buffer the data when ready to transmit
    always @(posedge clock) begin
        if (rst) begin 
            tx_data_buffer_i <= 0; 
        end
        else if (tx_data_valid && tx_data_ready_i) begin 
            tx_data_buffer_i <= tx_data_in;
        end
    end

    assign tx_data_ready_i = (current_state == IDLE);
    assign tx_ready = tx_data_ready_i;


    // the baud counter
    always @(posedge clock) begin 
        if (rst) begin
            tx_baud_counter_i <= 'd0;
        end
        else begin 
            if ( tx_baud_done_i || current_state != next_state) begin 
                tx_baud_counter_i <= 'd0;
            end
            else begin
                tx_baud_counter_i <= tx_baud_counter_i + 'd1;
            end 
        end
    end

    assign tx_baud_done_i = tx_baud_counter_i == BAUD_COUNTER_MAX - 1;

    // the data bit counter.
    always @(posedge clock) begin
        if ( rst ) begin 
            tx_data_counter_i <= 'd0;
        end
        else if (tx_baud_done_i) begin 
            if ( current_state != next_state) begin 
                tx_data_counter_i <= 'd0;
                tx_data_shiftreg <= tx_data_buffer_i;
            end
            else begin 
                tx_data_counter_i <= tx_data_counter_i + 1;
                tx_data_shiftreg <= tx_data_shiftreg >> 1;
            end 
        end 
    end

    assign tx_data_done_i = tx_data_counter_i == DATA_COUNTER_MAX - 1;

    // state machine (combinatorial)
    always @(*) begin 
        case (current_state)
            IDLE: begin 
                if (tx_data_valid) begin 
                    next_state = START;
                end
                else begin 
                    next_state = current_state;
                end
            end

            START: begin 
                if ( tx_baud_done_i) begin
                    next_state = DATA;
                end
                else begin 
                    next_state = current_state;
                end
            end

            DATA: begin
                if (tx_baud_done_i && tx_data_done_i) begin 
                    next_state = PARITY;
                end
                else begin 
                    next_state = current_state;
                end
            end

            PARITY: begin 
                if ( tx_baud_done_i) begin 
                    next_state = STOP;
                end
                else begin 
                    next_state = current_state;
                end
            end

            STOP: begin
                if ( tx_baud_done_i) begin 
                    next_state = WAIT;
                end
                else begin 
                    next_state = current_state;
                end
            end

            WAIT: begin 
                if ( tx_baud_done_i) begin 
                    next_state = IDLE;
                end
                else begin 
                    next_state = current_state;
                end
            end

            default: next_state = current_state;
        endcase
    end 

    // state transitions. (mealy outputs would be determined here)
    always @(posedge clock) begin 
        if ( rst) begin 
            current_state <= IDLE;
        end
        else begin 
            current_state <= next_state;
        end
    end

    // moore outputs
    always @(*) begin 
        case (current_state)
            IDLE: begin 
                tx_data_out_i = TX_IDLE;
            end

            START: begin 
                tx_data_out_i = TX_START;
            end

            DATA: begin 
                tx_data_out_i = tx_data_shiftreg[0];
            end

            PARITY: begin 
                tx_data_out_i = (EVEN_PARITY)? ^tx_data_buffer_i: ~^tx_data_buffer_i;
            end

            STOP: begin 
                tx_data_out_i = TX_WAIT;
            end

            WAIT: begin 
                tx_data_out_i = TX_WAIT;
            end 
            default: tx_data_out_i = TX_IDLE;
        endcase 
    end

    assign tx_data_out = tx_data_out_i;

    
endmodule