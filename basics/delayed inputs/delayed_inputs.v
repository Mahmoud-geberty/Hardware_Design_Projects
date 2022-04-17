module delayed_inputs (
    input i_clk, reset, i_kill_clr,
          i_go1, i_kill1, i_go2, i_kill2, 
          i_go3, i_kill3, 
    output o_done1, o_done2, o_done3, output reg o_kill_ltchd     
);

    counter_sm  go_delay_1 (
        .i_clk(i_clk), 
        .reset(reset),
        .i_go(i_go1), 
        .i_kill(i_kill1),
        .o_done(o_done1)
    );

    counter_sm  go_delay_2 (
        .i_clk(i_clk), 
        .reset(reset),
        .i_go(i_go2), 
        .i_kill(i_kill2),
        .o_done(o_done2)
    );

    counter_sm  go_delay_3 (
        .i_clk(i_clk), 
        .reset(reset),
        .i_go(i_go3), 
        .i_kill(i_kill3),
        .o_done(o_done3)
    );

    // SR latch for o_kill_ltchd signal
    always @(posedge i_clk, posedge reset) begin
        if (reset) begin
            o_kill_ltchd <= 1'b0;
        end else if (i_kill_clr) begin
            o_kill_ltchd <= 1'b0; 
        end else if (i_kill1 || i_kill2 || i_kill3) begin 
            o_kill_ltchd <= 1'b1;
        end
    end
endmodule