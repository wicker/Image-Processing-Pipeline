// This file defines the testbench and testing interface for a camera module
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
// contains testbench and testing interface
//

interface test_bus(input bit ref_clk);

  import defs::*;

  logic reset, dsp_clk;
  logic pixel_in_rdy, pixel_out_rdy;
  pixel_t pixel_in, pixel_out;
  flags_t flags;

  modport tb   (input ref_clk, pixel_in, pixel_in_rdy, 
                output pixel_out, pixel_out_rdy, reset, flags);

  modport dut  (input pixel_in, pixel_in_rdy, ref_clk, reset, flags,
                output pixel_out, pixel_out_rdy);

endinterface

// other options:
// fill an array with random coefficient combinations and generate 5 test cases based on that
// fill an array with random coefficeints on a range to test range of each convolution operation
// get results for convolution combinations
// add a monitor and concurrent assertions
// add basic checker for straight-through pipeline check
// TBX to send one testcase through - at least synthesis

module testbench(test_bus tbus);

  import defs::*;

  // this setup will accept RGB inputs but will not operate on those values
  // it just passes them through along with the first channel values it's operating on

  test_array_t test_input[TESTCASES], test_output[TESTCASES];
  test_array_t test0,test1,test2,test3;

  int infile, a, input_ptr;

  // set the input file here. there's some weird $fopen fuckery happening 
  // so we can only pull from one base image for every simulation.

  string infilename = "input/input-brokentop.txt";
  int color_space = COLORSPACE;

  pixel_t source[TOTALPIXELS];

  initial begin

    // filename is the output text file in which to save resulting pixels
    // base coefficients are x, y, z
    // we're not interested in changing the operations (a,b,c,d,e etc)
    // we're interested in relative strength of operations (x*emboss, y*edge)

    // test0  : blurred with x = 1 (baseline beej)

    test0.filename = "output/output-baseline-blur.txt";
    test0.flags.height = 320;
    test0.flags.width  = 240;
    test0.flags.totalpix = 240*320;
    test0.flags.color_space = GRAY;
    test0.flags.blur = 1;
    test0.flags.blurval = 1;
    test0.flags.edges = 0;
    test0.flags.emboss = 0;

    // fill the test input and output arrays
    // will run through all of these

    test_input[0] = test0;

    test_output[0] = test0;

    // all the testcases will run on this one image
    // which will be loaded now into a test array 'source' 

    infile = $fopen("input/input-brokentop.txt", "r"); 

    if (!infile) begin
      $display("Could not open the input file.");
    end

    // int 'a' is the number of pixels
    else begin
      while (!$feof(infile)) begin
        if (color_space == GRAY) begin 
          input_ptr = $fscanf(infile, " %d", source[a].y);
          a++;
        end
        else begin
          input_ptr = $fscanf(infile, " %d %d %d", source[a].y,source[a].u,source[a].v);
        end
      end
    end

    tbus.reset = 1;
    #1
    tbus.reset = 0;

    $fclose(infile);

  end

  //
  // for each test case, 
  // apply reset high to clear the pipeline
  // apply new flags 
  // pixel by pixel, read data from source image in and out of the pipeline
  // save into appropriate test_output
  //

  always_ff @(posedge tbus.ref_clk) begin

    static int din = 0;     // input pixel ('data in')
    static int dout = 0;    // output pixel ('data out')
    static int outfile, i;
    static int pixout = 0;
    static logic flag_rst = 0;

    // if this is the first event in this case

    if (i < TESTCASES) begin

      if (din == 0) begin
        if (!tbus.reset && !flag_rst) begin     // reset not asserted and flag not set - assert reset
          tbus.reset = 1;
          tbus.flags = test_input[i].flags;
          flag_rst = 0;
          tbus.pixel_in_rdy = 0;
        end
        else if (tbus.reset == 1 && !flag_rst) begin // reset asserted but flag not set - deassert reset
          tbus.reset = 0;
          flag_rst = 1;
          tbus.pixel_in_rdy = 0;
        end
        else if (!tbus.reset && flag_rst) begin // reset not asserted but flag set - ready to send
          tbus.pixel_in = source[din];
          tbus.pixel_in_rdy = 1;
          din++;
        end
      end

      // that was the last pixel so signal that we have no more by deasserting data ready

      else if (din == test_input[i].flags.totalpix) begin
        tbus.pixel_in_rdy = 0;
      end

      // neither the first nor last pixel so pass on din

      else begin
        tbus.pixel_in = source[din];
        tbus.pixel_in_rdy = 1;
        din++;
      end
      
      // if a pixel is waiting on the output end of the DSP pipeline,
      // scoop it up into the test_output array.
      
      if (tbus.pixel_out_rdy == 1) begin
        test_output[i].data[dout] = tbus.pixel_out;
        a2: assert($isunknown(tbus.pixel_out.y) == 0)
        else $warning("In case %0d pixel %0d is unknown.",i,dout);
        dout++;
      end

      // if that was the last pixel we're expecting
      // read out all of the data for this testcase 
      // then reset the flags

      if (dout == test_input[i].flags.totalpix) begin

        outfile = $fopen(test_output[i].filename, "w");

        if (!outfile) $display("Could not open the output file.");
        else begin
          while (pixout != test_input[i].flags.totalpix) begin
            if (test_output[i].flags.color_space == GRAY) begin
              $fwrite(outfile,"%d ", test_output[i].data[pixout].y);
              pixout++;
            end
            else begin
              $fwrite(outfile,"%d %d %d ", test_output[i].data[pixout].y, test_output[i].data[pixout].u, test_output[i].data[pixout].v);
             pixout++;
            end
          end
        end

        $fclose(outfile);

//      foreach (test_input[i]) begin
//        if (test_input[i].flags.edges == 0 && test_input[i].flags.blur == 0 && test_input[i].flags.emboss == 0) begin
//          a1: assert (source[a+1].y == test_input[i].data[a].y) a++;
//          else $error("Basic pipeline failure in case %0d: output doesn't match input in no-operation state.",i);
//        end
//      end

        i++;
        tbus.reset = 0;
        flag_rst = 0;
        din = 0;
        dout = 0;
        pixout = 0;
      end
    end

    else begin
      $finish;
    end

  end

  final begin

    $display("Testbench finished.");

  end

endmodule

