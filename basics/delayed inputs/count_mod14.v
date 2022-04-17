module count_mod14 (
    input    clk, stop, start, reset,
    output  reg [3:0] count, output reg stop_d2
);
    reg r_stop_d1, r_cnt_en;

    // latch the start signal, so it only needs to persist
    // for one clock edge and the counter will keep counting 
    // until the stop signal latches.
    always @(posedge clk , posedge reset) begin
        if (reset) begin 
            r_cnt_en <= 0;
        end 
        else if (stop) begin
            r_cnt_en <= 0;
        end
        else if (start) begin 
            r_cnt_en <= 1'b1;
        end
    end

    // counter procedure
    always @(posedge clk, posedge reset) begin
        if (reset) begin 
            count <= 0;
        end
        else if (r_cnt_en) begin
            if (count == 4'd13) begin 
                // reset (modulo behaviour)
                count <= 0;
            end else begin 
                count <= count + 1;
            end 
        end
    end

    // delay stop output by one clock
    always @(posedge clk, posedge reset) begin 
        if (reset) begin 
            r_stop_d1 <= 0;
            stop_d2 <= 0;
        end
        else if (stop) begin 
            r_stop_d1 <= stop;
            stop_d2 <= r_stop_d1;
        end
    end

endmodule