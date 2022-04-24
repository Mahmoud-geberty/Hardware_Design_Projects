module counter (
    input  T, clk, rst,
    output tr, tg_small, tg_main, ty, 
    output reg [4:0] count
);

// reg       count_internal_rst; 

// the counter
always @(posedge clk, posedge rst) begin
    if (rst) begin
        count <= 0; 
    end
    else if (T) begin 
        // T is just a synchronous reset
        count <= 0; 
    end
    else begin 
        count <= count + 1; 
    end
end

// // the counter enable 
// always @(posedge clk) begin 
//     if (T) begin 
//         count_internal_rst <= 1; 
//     end
//     else begin 
//         count_internal_rst <= 0; 
//     end
// end 


assign tr = count >= 5'd3;
assign ty = count >= 5'd8;
assign tg_small = count >= 5'd13;
assign tg_main = count >= 5'd18;


endmodule