// This file defines the wrapper for the entire project
//  
// Author   : Jenner Hanni <wicker>
// Email    : jeh.wicker@gmail.com
//
// Date     : 4-June-2013 
// Language : SystemVerilog 
// Class    : ECE510 -- Professor Mark Faust 
//
// License  : 3-clause BSD
// Read it at <http://opensource.org/licenses/BSD-3-Clause>
//  
// contains the top module that instantiates the dut, testing interface, and testbench
//

module top();

  import defs::*;
  
  logic ref_clk;

  // test_bus  connects the dut and the testbench
  test_bus bus(ref_clk);

  // testbench generates array of input pixels
  //           contains behavioral modules
  //           checks the dut output against the behavioral output
  //           maintains the scoreboard
  testbench tb(.tbus(bus)); // contains generate, and checker

  // synthesizable dut containing the image processing elements
  design_under_test dut(.tbus(bus));

  // start the reference clock
  initial begin
    ref_clk = FALSE;
    forever #REF_CLK_WIDTH ref_clk = ~ref_clk;
  end

endmodule


