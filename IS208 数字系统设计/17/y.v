`timescale 1 ns / 1 ns

module mealy(
	output reg flag,
	input din, clk, rst
);

reg [2:0] state;

parameter A = 3'b000;
parameter B = 3'b001;
parameter C = 3'b010;
parameter D = 3'b011;
parameter E = 3'b100;
parameter F = 3'b101;
parameter G = 3'b110;
parameter M = 3'bXXX;

always @(posedge clk or posedge rst) begin
	if (rst) begin
		flag <= 1'b0;
		state <= M;
	end	else begin
	  	case (state)
			A: begin
				flag <= 1'b0;
				state <= din ? B : A;
			end
			B: begin
				flag <= 1'b0;
				state <= din ? M : C;
			end
			C: begin
				flag <= 1'b0;
				state <= din ? D : A;
			end
			D: begin
				flag <= 1'b0;
				state <= din ? M : E;
			end
			E: begin
				flag <= 1'b0;
				state <= din ? F : A;
			end
			F: begin
				flag <= 1'b0;
				state <= din ? M : G;
			end
			G: begin
				flag <= din ? 1'b1 : 1'b0;
				state <= din ? F : A;
			end
			default: begin
				flag <= 1'b0;
				state <= din ? M : A;
			end
		endcase
	end
end

endmodule
