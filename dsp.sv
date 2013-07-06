// This file defines DSP-specific files
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
// ----------------------------------------------------- 
//
// This file defines the DSP modules
// See documentation for flags struct:
//
// -- dsp_blur (data_bus dbus, clock_bus cbus);
// -- dsp_edges (data_bus dbus, clock_bus cbus);
// -- dsp_emboss (data_bus dbus, clock_bus cbus);
// -- dsp_flip (data_bus dbus, clock_bus cbus);
// -- dsp_invert (data_bus dbus, clock_bus cbus);

module dsp_blur (data_bus dbus, clock_bus cbus);

  import defs::*;
  import ops::*;

  int width, height, lastpixel;
  int totalpix = TOTALPIXELS;

  // calculating blur coefficients
  // this is basically the most simple coefficient matrix ever
  //
  // [1 1 1   [a b c    set value of blurval during test generation 
  //  1 1 1    d e f    note: setting blurval > 1 lightens
  //  1 1 1];  g h i];        setting blurval < 1 darkens
  //
  // see? easy.
  //

  int a,b,c,d,e,f,g,h,i;

  int poi = 1;

  assign width = dbus.flags.width;
  assign height = dbus.flags.height;
  pixel_t image[TOTALPIXELS+1];

  always_comb begin
    if (dbus.reset == 1) begin
      a = dbus.flags.blurval;
      b = dbus.flags.blurval;
      c = dbus.flags.blurval;
      d = dbus.flags.blurval;
      e = dbus.flags.blurval;
      f = dbus.flags.blurval;
      g = dbus.flags.blurval;
      h = dbus.flags.blurval;
      i = dbus.flags.blurval;
    end
  end

  // can't update pixel of interest until lastpixel = width + 2
  always_ff @(posedge cbus.dsp_clk) begin 

    if (!dbus.flags.blur) begin
      if (dbus.blur_dinrdy) begin
        dbus.blur_dout = dbus.blur_din;
        dbus.blur_doutrdy = 1;
      end
      else begin
        dbus.blur_doutrdy = 0;
      end
    end

    else begin // perform the operation in this module

      // if the dinrdy signal is set, we have an input pixel
      if (dbus.blur_dinrdy) begin
        image[lastpixel] = dbus.blur_din;
        lastpixel++;
      end

     // do we have enough pixels in the array to build a kernel?
      // if not enough pixels in array to build kernel, wait for another pixel.
      if (lastpixel < (width + 3)) begin
        dbus.blur_doutrdy = 0;
      end

      // else we have enough. do the convolution and pass resulting pixel to output
      else begin 
        dbus.blur_dout = convolution(a,b,c,d,e,f,g,h,i, poi, height, width, image);
        dbus.blur_doutrdy = 1;
        if (poi <= totalpix) 
          poi++;
      end
    end
  end

endmodule

module dsp_edges (data_bus dbus, clock_bus cbus);

  import defs::*;
  import ops::*;

  int width, height, lastpixel;
  int totalpix = TOTALPIXELS;

  // calculating edge coefficients
  //
  // [-18 -9 0    [-2x -x 0       [a b c    
  //  -9  9  9      -x  x x        d e f    
  //  0   9  18];    0  x 2x]      g h i]; 
  //
  // set x as the edgeval and everything else is in terms of that
  //

  int a,b,c,d,e,f,g,h,i;

  int poi = 1;

  assign width = dbus.flags.width;
  assign height = dbus.flags.height;
  pixel_t image[TOTALPIXELS+1];

  always_comb begin
    if (dbus.reset == 1) begin
      a =  0 * dbus.flags.edgesval;
      b =  1 * dbus.flags.edgesval;
      c =  0 * dbus.flags.edgesval;
      d =  1 * dbus.flags.edgesval;
      e = -4 * dbus.flags.edgesval;
      f =  1 * dbus.flags.edgesval;
      g =  0 * dbus.flags.edgesval;
      h =  1 * dbus.flags.edgesval;
      i =  0 * dbus.flags.edgesval;
    end
  end

  // can't update pixel of interest until lastpixel = width + 2 
  always_ff @(posedge cbus.dsp_clk) begin 

    if (!dbus.flags.edges) begin
      if (dbus.edges_dinrdy) begin
        dbus.edges_dout = dbus.edges_din;
        dbus.edges_doutrdy = 1;
      end
      else begin
        dbus.edges_doutrdy = 0;
      end
    end

    else begin // perform the operation in this module
      // if the dinrdy signal is set, we have an input pixel
      if (dbus.edges_dinrdy) begin
        image[lastpixel] = dbus.edges_din;
        lastpixel++;
      end

      // regardless of input pixel, test to see if we have enough pixels in the array to build a kernel
      // if not enough pixels in array to build kernel. wait for another pixel.
      if (lastpixel < (width + 3)) begin
        dbus.edges_doutrdy = 0;
      end

      // else we have enough. do the convolution and pass resulting pixel to output
      else begin
        dbus.edges_dout = convolution(a,b,c,d,e,f,g,h,i, poi, height, width, image);
        dbus.edges_doutrdy = 1;
        if (poi <= totalpix) 
          poi++;
      end
    end
  end


endmodule

module dsp_emboss (data_bus dbus, clock_bus cbus);

  import defs::*;
  import ops::*;

  int width, height, lastpixel;
  int totalpix = TOTALPIXELS;

  // calculating emboss coefficients
  //
  // [-18 -9 0    [-2x -x 0       [a b c    
  //  -9  9  9      -x  x x        d e f    
  //  0   9  18];    0  x 2x]      g h i]; 
  //
  // set x as the embossval and everything else is in terms of that
  //

  int a,b,c,d,e,f,g,h,i;

  int poi = 1;

  assign width = dbus.flags.width;
  assign height = dbus.flags.height;
  pixel_t image[TOTALPIXELS+1];

  always_comb begin
    if (dbus.reset == 1) begin
      a = -2 * dbus.flags.embossval;
      b = -1 * dbus.flags.embossval;
      c =  0 * dbus.flags.embossval;
      d = -1 * dbus.flags.embossval;
      e =  1 * dbus.flags.embossval;
      f =  1 * dbus.flags.embossval;
      g =  0 * dbus.flags.embossval;
      h =  1 * dbus.flags.embossval;
      i =  2 * dbus.flags.embossval;
    end
  end

  // can't update pixel of interest until lastpixel = width + 2
  always_ff @(posedge cbus.dsp_clk) begin 

    if (!dbus.flags.emboss) begin
      if (dbus.emboss_dinrdy) begin
        dbus.emboss_dout = dbus.emboss_din;
        dbus.emboss_doutrdy = 1;
      end
      else begin
        dbus.emboss_doutrdy = 0;
      end
    end

    else begin // perform the operation in this module
      // if the dinrdy signal is set, we have an input pixel
      if (dbus.emboss_dinrdy) begin
        image[lastpixel] = dbus.emboss_din;
        lastpixel++;
      end

      // regardless of input pixel, test to see if we have enough pixels in the array to build a kernel
      // if not enough pixels in array to build kernel. wait for another pixel.
      if (lastpixel < (width + 3)) begin
        dbus.emboss_doutrdy = 0;
      end

      // else we have enough. do the convolution and pass resulting pixel to output
      else begin
        dbus.emboss_dout = convolution(a,b,c,d,e,f,g,h,i, poi, height, width, image);
        dbus.emboss_doutrdy = 1;
        if (poi <= totalpix) 
          poi++;
      end
    end
  end

endmodule

module dsp_flip (data_bus dbus, clock_bus cbus);

  import defs::*;

  pixel_t pixel;

  always_ff @(posedge cbus.dsp_clk) begin
    if (dbus.flip_dinrdy) begin
      pixel = dbus.flip_din;
      dbus.flip_dout = pixel;
      dbus.flip_doutrdy = 1;
    end
    else begin
      dbus.flip_doutrdy = 0;
    end
  end

endmodule

module dsp_invert (data_bus dbus, clock_bus cbus);

  import defs::*;

  pixel_t pixel;

  always_ff @(posedge cbus.dsp_clk) begin
    if (dbus.invert_dinrdy) begin
      pixel = dbus.invert_din;
      dbus.invert_dout = pixel;
      dbus.invert_doutrdy = 1;
    end
    else begin
      dbus.invert_doutrdy = 0;
    end
  end

endmodule

