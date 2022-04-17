module counter_sm (
    input i_clk, i_kill, i_go, reset,
    output reg o_done, output reg [6:0] r_count,
    // outputs only for viewing on the wave
    output reg o_idle, o_active, o_abort, o_finish
);

    // delcare internal wires and regs
    reg [1:0] r_state;

    // state parameter
    parameter s_idle   = 2'b00;
    parameter s_active = 2'b01;
    parameter s_abort  = 2'b10;
    parameter s_finish = 2'b11;

    // state machine
    always @(posedge i_clk, posedge reset) begin
        if (reset ) begin 
            r_state = s_idle;
        end
        else begin 
            case (r_state)
                s_idle: 
                begin 
                    if (i_go) begin 
                        r_state <= s_active;
                    end
                end

                s_active:
                begin 
                    if (i_kill) begin 
                        r_state <= s_abort;
                    end 
                    else if (r_count == 7'd100) begin
                        r_state <= s_finish;
                    end
                end

                s_abort:
                begin 
                    if (!i_kill) begin 
                        r_state <= s_idle;
                    end 
                end

                s_finish:
                begin 
                    r_state <= s_idle;
                end

                default: 
                begin 
                    r_state <= s_idle;
                end
            endcase
        end
    end

    // counter 
    always @(posedge i_clk, posedge reset)
    begin
        if (reset)
        begin
            r_count = 6'd0;
        end
        else if (r_state == s_abort || r_state == s_finish)
        begin
            r_count = 6'd0;
        end
        else if (r_state == s_active)
        begin
            r_count <= r_count + 6'd1;
        end
    end

    // done register
    always @(posedge i_clk, posedge reset) begin
       if (reset) begin 
           o_done <= 1'b0;
       end 
       else if (r_state == s_finish) begin 
           o_done <= 1'b1;
       end
       else begin
           o_done <= 1'b0;
       end
    end

    // show the states in the wave
    always @(posedge i_clk, posedge reset) begin
        if (reset) begin
            o_idle <= 0;
            o_active <= 0;
            o_finish <= 0;
            o_abort <= 0;
        end
        else if (r_state == s_idle) begin
            o_idle <= 1;
            o_active <= 0;
            o_finish <= 0;
            o_abort <= 0;
        end
        else if (r_state == s_active) begin
            o_active <= 1;
            o_idle <= 0;
            o_finish <= 0;
            o_abort <= 0;
        end
        else if (r_state == s_finish) begin
            o_finish <= 1;
            o_abort <= 0;
            o_idle <= 0;
            o_active <= 0;
        end
        else if (r_state == s_abort) begin
            o_abort <= 1;
            o_idle <= 0;
            o_active <= 0;
            o_finish <= 0;
        end
    end

endmodule