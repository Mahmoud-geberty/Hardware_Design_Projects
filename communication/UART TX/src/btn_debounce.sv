module btn_debounce #(
    CLK_RATE = 100000000
) (
    input  clock,
    input  rst,
    input  btn,
    output tx_send
    //output btn_detect,
    //output [BTN_DELAY_SIZE - 1: 0] delay_counter_i
);

    // delay for 100ms, (1/100m) = 10
    localparam BTN_DELAY_MAX = CLK_RATE/10;
    localparam BTN_DELAY_SIZE = $clog2(BTN_DELAY_MAX);

    // internal signals and registers
    logic                         btn_detect_i; 
    logic                         delay_done_i;
    logic [BTN_DELAY_SIZE - 1: 0] delay_counter;

    initial begin
        btn_detect_i <= 0; 
        delay_counter <= 0; 
    end

    // btn edge detector flip flop
    always @(posedge clock, posedge rst) begin 
        if (rst || delay_done_i) begin 
            btn_detect_i <= 'd0;
        end
        else if (btn) begin 
            btn_detect_i <= 'd1;
        end
    end

    //assign btn_detect = btn_detect_i;

    // delay counter 
    always @(posedge clock, posedge rst) begin 
        if (rst || delay_done_i) begin
            delay_counter <= 'd0;
        end
        else if (btn_detect_i) begin 
            delay_counter <= delay_counter + 'd1;
        end
    end
    assign delay_done_i = (delay_counter == BTN_DELAY_MAX - 1);

    assign tx_send = delay_done_i; // tx_send is high for one pulse
    
    
endmodule