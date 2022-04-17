module spi_master(
  input logic [7:0]  i_message,
  input logic        i_clock,
  input logic        i_send,

  output logic       o_msgBit,
  output logic       o_done,
  output logic       o_sclk,
  output logic [7:0] dbg_msg
);
  
  logic [7:0]  r_message; // copy the message from the port to internal register.
  logic [3:0]  r_bitCount;   // keep track of number of bits sent.
  logic        r_sendDetect; // detect a rising edge of the i_send signal.

  always_ff @(posedge i_clock) begin
    if (i_send && ~r_sendDetect) begin 
      r_sendDetect <= 1'b1;
      r_message <= i_message;
      o_done <= 0;
    end
    else if (~i_send && ~r_sendDetect) begin 
      r_sendDetect <= 1'b0; // prevent an inferred latch. 
    end

    if (r_sendDetect) begin 
      if (~o_sclk) begin 
        o_sclk = (r_bitCount >= 4'h1)?  1'b1 : 1'b0;
        r_bitCount = r_bitCount + 1;
        o_done <= 0;
      end
      else if (o_sclk ) begin 
        o_sclk = 1'b0;
        o_msgBit = r_message[0];  // prepare the data bit before rising edge of sclk.
        r_message <= r_message >> 1;
      end

      // make sure initial bit is ready in time for first o_sclk rising edge.
      if (r_bitCount == 1 ) begin 
        o_msgBit = r_message[0];  // prepare the data bit before rising edge of sclk.
        r_message <= r_message >> 1;
      end 

      if (r_bitCount == 4'h9) begin // counting to 9 because we used the first count to prepare the values
        r_sendDetect <= '0;
        o_done <= 1;
        r_bitCount = '0;
      end
    end
  end

  wire [7:0] msg;
  assign msg = r_message;
  assign dbg_msg = msg;

endmodule