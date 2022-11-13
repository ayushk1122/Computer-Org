// 4-bit up/down counter
module Count4(clk,rst,enable,upDown, count);
   input clk, rst, enable, upDown;
   output reg [3:0] count;

   // insert code here 
   always @ (posedge clk)
   begin 
      if (rst) // Asynchronous active high reset
         count <= 4'b0;
      else
         begin 
            if (upDown && enable) // Count up
               count <= count + 1'b1;
            else if(!upDown && enable)// Count down
               count <= count - 1'b1;
         end
   end

endmodule  // Count4