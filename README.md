Image Processing with Convolution 
=================================

Jenner Hanni  
<jeh.wicker@gmail.com>

**SystemVerilog**

This project performs pixel-by-pixel convolution on a 2-dimensional image of any height and width. A user-defined type pixel_t is a three-channel struct which is passed onto the data bus and through the decoder, blur, edges, emboss, flip, invert, and encoder modules. The only modules which perform operations are blur, edges, and emboss. Download the files and `run` the script.

Compiled using Mentor Questa.

**VHDL**

This is taking the learnings from the SV implementation and bringing up a testbed for image processing algorithm modules on the Altera Cyclone IV. New as of 25 Sep 2013. 

Released under the 3-clause BSD License.
