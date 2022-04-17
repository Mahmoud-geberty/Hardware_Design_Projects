module spi_hd_txonly(
  input logic  i_btn,
  input logic  i_clock,

  output logic o_msgBit,
  output logic o_sclk,
  output logic [7:0] dbg_msg
);

wire [1:0] w_raddr;
wire       w_send;
wire       w_done;

spi_cu cu(
  .i_done(w_done),
  .o_raddr(w_raddr),
  .o_send(w_send),
  .*
);

spi_du du(
  .o_done(w_done),
  .i_raddr(w_raddr),
  .i_send(w_send),
  .*
);
  
endmodule