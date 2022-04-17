module spi_TXreg(
  input logic [1:0]  i_raddr,
  input logic        i_clock,

  output logic [7:0] o_msg
);
  
  typedef logic [7:0] byte_t;
  byte_t [3:0] r_msgFile;

  assign r_msgFile[0] = 8'hFF;
  assign r_msgFile[1] = 8'hCE;
  assign r_msgFile[2] = 8'h18;
  assign r_msgFile[3] = 8'h87;

  always_ff @(posedge i_clock) begin
    o_msg = r_msgFile[i_raddr];
  end
endmodule