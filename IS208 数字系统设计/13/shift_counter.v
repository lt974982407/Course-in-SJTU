`timescale 1 ns / 1 ns

module shift_counter(
	output reg [7:0] count,
	input clk, reset
);

reg [4:0] state;

initial state = 5'b0;

always @(posedge clk or posedge reset) begin
    if (reset)
        state <= 3'b0;
    else begin
        if (state == 5'b10001)
            state <= 5'b00011;
        else 
            state <= state + 1;
    end     
end

always @(*) begin
    case (state)
    5'b00000 : count = 8'b00000001;
    5'b00001 : count = 8'b00000001;
    5'b00010 : count = 8'b00000001;
    5'b00011 : count = 8'b00000001;
    5'b00100 : count = 8'b00000010;
    5'b00101 : count = 8'b00000100;
    5'b00110 : count = 8'b00001000;
    5'b00111 : count = 8'b00010000;
    5'b01000 : count = 8'b00100000;
	5'b01001 : count = 8'b01000000;
	5'b01010 : count = 8'b10000000;
	5'b01011 : count = 8'b01000000;
	5'b01100 : count = 8'b00100000;
	5'b01101 : count = 8'b00010000;
	5'b01110 : count = 8'b00001000;
	5'b01111 : count = 8'b00000100;
	5'b10000 : count = 8'b00000010;
	5'b10001 : count = 8'b00000001;
    endcase
end

endmodule
