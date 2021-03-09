  
`timescale 1 ns / 1 ns

module counter8b_updown(output reg [7:0] count, input clk, reset, dir);

initial 
    count = 8'b0;

always @(posedge clk or posedge reset) begin
    if(reset)
        count <= 8'b0;
    else begin
        if(dir)
            count <= count+1;
        else
            count <= count-1; 
    end
end

endmodule
