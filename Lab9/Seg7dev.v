module bin7seg (x, seg, dp);
	input [3:0]x;     //4-bit input to display
	output [6:0]seg;    //segments form a to g
	output dp;         //decimal points
	reg[6:0]seg;       //re-declare as the type of reg
	
	always @ (x)
		case(x)
			0: seg = 7'b0000001;
			1: seg = 7'b1001111;
			2: seg = 7'b0010010;
			3: seg = 7'b0000110;
			4: seg = 7'b1001100;
			5: seg = 7'b0100100;
			6: seg = 7'b0100000;
			7: seg = 7'b0001111;
			8: seg = 7'b0000000;
			9: seg = 7'b0000100;
			10: seg = 7'b0001000;
			11: seg = 7'b1100000;
			12: seg = 7'b0110001;
			13: seg = 7'b1000010;
			14: seg = 7'b0110000;
			15: seg = 7'b0111000;
			default: seg = 7'b1111110;
		endcase
endmodule