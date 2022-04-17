module spi_du(
  input logic        i_clock,
  input logic        i_send,
  input logic  [1:0] i_raddr,

  output logic       o_done,
  output logic       o_msgBit,
  output logic       o_sclk,
  output logic [7:0] dbg_msg
);

wire w_done;
wire w_msgBit;
wire w_sclk;

wire [7:0]  w_message;

spi_master spi (
  .i_message(w_message),
  .o_msgBit(w_msgBit),
  .o_sclk(w_sclk),
  .o_done(w_done),
  .i_clock,
  .i_send,
  .*
);

spi_TXreg tx (
  .o_msg(w_message),
  .*
);

assign o_done = w_done;
assign o_sclk = w_sclk;
assign o_msgBit = w_msgBit;
  
endmodule