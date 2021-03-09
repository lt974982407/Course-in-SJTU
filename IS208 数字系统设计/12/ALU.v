`define OP_and			3'b000
`define OP_substract	3'b001
`define OP_substract_a	3'b010
`define OP_or_ab		3'b011
`define OP_and_ab		3'b100
`define OP_not_ab		3'b101
`define OP_exor			3'b110
`define OP_exnor		3'b111

module ALU(
	output reg c_out,
	output reg [7:0] sum,
	input [2:0] oper,
	input [7:0] a,
	input [7:0] b,
	input c_in
);

always @(*) begin
	case (oper)
		`OP_and			: {c_out, sum} = a + b + c_in;
		`OP_substract	: {c_out, sum} = a + ~b + c_in;
		`OP_substract_a	: {c_out, sum} = b + ~a + ~c_in;
		`OP_or_ab		: {c_out, sum} = {1'b0, a | b};
		`OP_and_ab		: {c_out, sum} = {1'b0, a & b};
		`OP_not_ab		: {c_out, sum} = {1'b0, (~a) | b};
		`OP_exor		: {c_out, sum} = {1'b0, a ^ b};
		`OP_exnor		: {c_out, sum} = {1'b0, a ~^ b};
		default: {c_out, sum} = 9'bx;
	endcase
end

endmodule
