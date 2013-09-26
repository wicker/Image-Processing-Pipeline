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
// contains dut, data bus, clock bus, decoder fifo, encoder fifo, and timegen
//

module design_under_test(test_bus tbus);

  import defs::*;

  clock_bus    cbus(ref_clk);
  data_bus     dbus(.cbus);

  timegen      tg(.dbus,.cbus);
  decoder_fifo decfifo(.dbus,.cbus);
  encoder_fifo encfifo(.dbus,.cbus);

  dsp_blur dblur(.dbus, .cbus);
  dsp_edges dedges(.dbus, .cbus);
  dsp_emboss demboss(.dbus, .cbus);
  dsp_flip dflip(.dbus, .cbus);
  dsp_invert dinvert(.dbus, .cbus);

  always_ff @(posedge tbus.reset) begin
    dbus.flags = tbus.flags;
    dbus.reset = tbus.reset;
    dbus.decfifo_doutrdy = 0;
    dbus.encfifo_doutrdy = 0;
    dbus.edges_dinrdy = 0;
    dbus.emboss_dinrdy = 0;
    dbus.blur_dinrdy = 0;
  end

  always_ff @(negedge tbus.reset) begin
    dbus.reset = tbus.reset;
  end

  // decide whether or not to step another pixel into the pipeline
  always_ff @(posedge cbus.dsp_clk) begin

    // if there's an input pixel ready, pass it into data bus
    if (tbus.pixel_in_rdy == 1) begin
      dbus.decfifo_din = tbus.pixel_in;
      dbus.decfifo_dinrdy = 1;
    end
    else begin
      dbus.decfifo_dinrdy = 0;
    end

    // if there's an output pixel ready, pass it out to the test bus
    if (dbus.encfifo_doutrdy == 1) begin
      tbus.pixel_out = dbus.encfifo_dout;
      tbus.pixel_out_rdy = 1;
    end
    else begin
      tbus.pixel_out_rdy = 0;
    end

  end

endmodule

// should only be one driver for each variable
// this bus supports a pipeline

interface data_bus(clock_bus cbus);

  import defs::*;

  logic reset;
  flags_t flags;

  // timegen is a special timing module we may or may not use

  pixel_t tg_din;
  logic tg_dinrdy;
  pixel_t tg_dout;
  logic tg_doutrdy;

  modport timegen (
    input tg_din,
    input tg_dinrdy,
    output tg_dout,
    output tg_doutrdy);

  // decoder FIFO, the DSP, and encoder FIFO are data path modules
  
  pixel_t decfifo_din;
  logic decfifo_dinrdy;
  pixel_t decfifo_dout;
  logic decfifo_doutrdy;

  pixel_t encfifo_din;
  logic encfifo_dinrdy;
  pixel_t encfifo_dout;
  logic encfifo_doutrdy;

  modport decoder_fifo (
    input reset,
    input flags,
    input decfifo_din,
    input decfifo_dinrdy,
    output decfifo_dout,
    output decfifo_doutrdy);

  modport encoder_fifo (
    input reset,
    input flags,
    input encfifo_din,
    input encfifo_dinrdy,
    output encfifo_dout,
    output encfifo_doutrdy);

  //
  // DSP-specific section
  // 

  pixel_t blur_din, blur_dout, edges_din, edges_dout; 
  pixel_t emboss_din, emboss_dout;
  pixel_t flip_din, flip_dout, invert_din, invert_dout;

  logic blur_dinrdy, blur_doutrdy, edges_dinrdy, edges_doutrdy;
  logic emboss_dinrdy, emboss_doutrdy;
  logic flip_dinrdy, flip_doutrdy, invert_dinrdy, invert_doutrdy;

  // step through the ready signals along with the data
  // pipeline: blur --> edges --> sharp --> emboss --> flip --> invert

  always_ff @(posedge cbus.dsp_clk) begin

    blur_dinrdy = decfifo_doutrdy;
    edges_dinrdy = blur_doutrdy;
    emboss_dinrdy = edges_doutrdy;
    flip_dinrdy = emboss_doutrdy;
    invert_dinrdy = flip_doutrdy;
    encfifo_dinrdy = invert_doutrdy;

    blur_din = decfifo_dout;
    edges_din = blur_dout;
    emboss_din = edges_dout;
    flip_din = emboss_dout;
    invert_din = flip_dout;
    encfifo_din = invert_dout;
  end

  // 
  // DSP module modports
  //

  modport dsp_blur (
    input reset,
    input flags,
    input blur_din,
    input blur_dinrdy,
    output blur_dout,
    output blur_doutrdy);
  
  modport dsp_edges (
    input reset,
    input flags,
    input edges_din,
    input edges_dinrdy,
    output edges_dout,
    output edges_doutrdy);
      
  modport dsp_emboss (
    input reset, 
    input flags,
    input emboss_din,
    input emboss_dinrdy,
    output emboss_dout,
    output emboss_doutrdy);
  
  modport dsp_flip (
    input reset,
    input flags,
    input flip_din,
    input flip_dinrdy,
    output flip_dout,
    output flip_doutrdy);
    
  modport dsp_invert (
    input reset,
    input flags,
    input invert_din,
    input invert_dinrdy,
    output invert_dout,
    output invert_doutrdy);
  
endinterface

interface clock_bus(logic ref_clk);

  import defs::*;

  logic dsp_clk, vid_clk;

  initial begin
    dsp_clk = FALSE;
    forever #REF_CLK_WIDTH dsp_clk = ~dsp_clk;
  end

  modport dsp_bus (input dsp_clk);

endinterface

module decoder_fifo (data_bus dbus, clock_bus cbus);

  import defs::*;

  always_ff @(posedge cbus.dsp_clk) begin
    if (dbus.reset) begin
      dbus.decfifo_dout = 0;
      dbus.decfifo_doutrdy = 0;
    end
    else if (dbus.decfifo_dinrdy == 1) begin
      dbus.decfifo_dout = dbus.decfifo_din;
      dbus.decfifo_doutrdy = 1;
    end
    else begin
      dbus.decfifo_doutrdy = 0;
    end
  end

endmodule

module encoder_fifo (data_bus dbus, clock_bus cbus);

  import defs::*;

  always_ff @(posedge cbus.dsp_clk) begin
    if (dbus.reset) begin
      dbus.encfifo_dout = 0;
      dbus.encfifo_doutrdy = 0;
    end
    else if (dbus.encfifo_dinrdy == 1) begin
      dbus.encfifo_dout = dbus.encfifo_din;
      dbus.encfifo_doutrdy = 1;
    end
    else begin
      dbus.encfifo_doutrdy = 0;
    end
  end

endmodule

// timegen is really a clocks module and has been pulled out of data path
// maybe will be deleted entirely -- not currently used, at least.

module timegen (data_bus dbus, clock_bus cbus);

  import defs::*;

  pixel_t pixel;

// always_ff @(posedge cbus.dsp_clk) begin
//   if (dbus.tg_dinrdy == 1) begin
//     pixel = dbus.tg_din;
//     dbus.tg_dout = pixel;
//     dbus.tg_doutrdy = 1;
//   end
//   else begin
//     dbus.tg_doutrdy = 0;
//   end
// end

endmodule


