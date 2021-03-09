
`timescale 1 ns / 1 ns

module moore(
	output reg flag,
	input din, clk, rst
);

reg [3:0] state;


parameter A = 4'b0000;
parameter B = 4'b0001;
parameter C = 4'b0010;
parameter D = 4'b0011;
parameter E = 4'b0100;
parameter F = 4'b0101;
parameter G = 4'b0110;
parameter H = 4'b0111;
parameter M = 4'b1000;

always @(posedge clk or posedge rst) begin
	if (rst == 1'b1)begin
	state <= M;
	end
	else begin 
        flag <= (state == H)? 1'b1: 1'b0;
		case (state)
		A: state <= din ? B : A;
		B: state <= din ? M : C;
		C: state <= din ? D : A;
		D: state <= din ? M : E;
		E: state <= din ? F : A;
		F: state <= din ? M : G;
		G: state <= din ? H : A;
		H: state <= din ? M : G;
		M: state <= din ? M : A;
		default: state <= M;
	endcase
end

end

endmodule

