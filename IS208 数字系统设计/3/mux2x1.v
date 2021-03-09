`timescale 1ns/1ns

module mux2x1(output dout, input sel, [1:0] din);
bufif1 a1(dout, din[1], sel);
bufif0 a2(dout, din[0], sel);

endmodule
