// Garage door opener FSM  
module Opener(            
   input clk,b,c,o,r,s,  // b: button, c: closed limit switch, o: open limit switch, r = reset, s = sensor
   output reg d,u,       // d: run the motor downwards to close the door, u: run the motor upwards to open the door
   output reg [1:0] State // an output for troubleshooting and simulation verification
);
 
 // insert code here
always @ (posedge clk)
begin
   if (r && c) 
      State = 3'd0;
   if (r && o) 
      State = 3'd2;
   if (r && !c && !o)
      State = 3'd1; 

   if ((State == 3'd0) && (b))
      State = 3'd1;     
   if ((State == 3'd1) && (o))
      State = 3'd2;
   if ((State == 3'd2) && (b) && (!s))
      State = 3'd3;
   else if ((State == 3'd3) && (s || b) && (!(c && !b && !s)))
      State = 3'd1;
   else if ((State == 3'd3) && (c && !b && !s) && (!(s || b)))
      State = 3'd0;

   if (State == 3'd1)
      u <= 1;

   if (State != 3'd1)
      u <= 0;

   if (State == 3'd3)
      d <= 1;

   if (State != 3'd3) 
      d <= 0;
end


   
endmodule