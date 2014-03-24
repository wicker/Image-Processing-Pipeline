Image Processing with Convolution 
=================================

Jenner Hanni  
<jeh.wicker@gmail.com>

Released under the 3-clause BSD License.

OpenCV
------

GetPixels: http://github.com/wicker/getpixels

Getpixels converts a color input image into a text file of its pixel values separated by spaces:

164 165 165 165 165 164 165 165 165 164 165 166 

It also converts a text file of pixel values back to an image.

1. ./GetPixels.o 1 brokentop.png brokentop.txt 240 320  
2. use SystemVerilog or C program to modify pixels
1. ./GetPixels.o 2 brokentop.png brokentop.txt 240 320  

SystemVerilog
-------------

This project performs pixel-by-pixel convolution on a 2-dimensional image of any height and width. A user-defined type pixel_t is a three-channel struct which is passed onto the data bus and through the decoder, blur, edges, emboss, flip, invert, and encoder modules. The only modules which perform operations are blur, edges, and emboss. Download the files and `run` the script.

Compiled using Mentor Questa.

C
-

See http://www.wickerbox.net/2014/03/convolution-in-c/ for more information.

