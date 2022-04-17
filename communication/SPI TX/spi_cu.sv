module spi_cu(
  input logic        i_clock,
  input logic        i_btn,
  input logic        i_done,

  output logic [1:0] o_raddr,
  output logic       o_send
);

parameter s_IDLE = 1'b0;
parameter s_TX   = 1'b1;

logic r_state;

logic r_btnDetected;
logic [2:0] r_counter;


always_ff @(posedge i_clock) begin
  case (r_state)
    s_IDLE:
    begin 
      if (r_btnDetected) begin 
        o_send <= '0;
        r_btnDetected <= '0;
        r_state <= s_TX;
      end else if (i_btn) begin 
        o_send <= 1'b1;
        r_btnDetected <= 1'b1;
        o_raddr <= '0;
        r_state <= s_IDLE;
      end
    end

    s_TX:
    begin 
      if (i_done && r_counter < 3'h4) begin 
        r_counter <= r_counter + 3'b1;
        o_send <= 1'b1;
        o_raddr <= r_counter + 3'b1;
        r_btnDetected <= 1'b1;
        r_state <= s_IDLE;
      end else if ( i_done ) begin
        r_counter <= '0;
        o_raddr <= '0;
        r_state <= s_IDLE;
      end else begin 
        r_state <= s_TX;
      end
    end

    default: r_state <= s_IDLE;
  endcase
end


  
endmodule