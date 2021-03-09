`timescale 1 ns / 1 ns

module seq_detect(
	output reg flag,
	input din,
	input clk,
	input rst_n
);

reg [2:0] current_state;
reg [2:0] next_state;

parameter X = 3'bxxx;
parameter A = 3'b000;
parameter B = 3'b001;
parameter C = 3'b010;
parameter D = 3'b011;
parameter E = 3'b100;
parameter F = 3'b101;
parameter G = 3'b110;
parameter H = 3'b111;

always @(negedge clk) begin
    current_state <= next_state;
end

always @(*) begin
    case (current_state)
 		A: next_state = din ? B : A;
		B: next_state = din ? C : A;
		C: next_state = din ? F : D;
		D: next_state = din ? H : A;
		E: next_state = din ? F : A;
		F: next_state = din ? F : G;
		G: next_state = din ? H : A;
		H: next_state = din ? C : A;
		default: next_state =  din ? E : A;
    endcase
end

always @(negedge clk) begin
	if(!rst_n)
		flag = 1'b0;
	else begin
		case (current_state)
			D: flag = 1'b1;
			H: flag = 1'b1;
			default: flag = 1'b0;
		endcase
	end
end

endmodule