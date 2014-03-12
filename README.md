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

1. ./GetPixels 1 brokentop.png brokentop.txt 240 320  
2. use SystemVerilog or C program to modify pixels
1. ./GetPixels 2 brokentop.png brokentop.txt 240 320  

SystemVerilog
-------------

This project performs pixel-by-pixel convolution on a 2-dimensional image of any height and width. A user-defined type pixel_t is a three-channel struct which is passed onto the data bus and through the decoder, blur, edges, emboss, flip, invert, and encoder modules. The only modules which perform operations are blur, edges, and emboss. Download the files and `run` the script.

Compiled using Mentor Questa.

C
-

This is a framework in C to either poll the user or accept arguments directly. The executables are 'main' (for polling) or 'script' (for arguments). 

To use main: 

./main input.txt

To use script:

./script input.txt width height x x1 y y1 output.txt

where 

input.txt is the location of the input text file  
width of the image in pixels  
height of the image in pixels  
x and y are convolution operations  
  1 - blur  
  2 - separable 2d  
  3 - custom 
x1 and y1 are the size N of the kernel NxN

Example:

./main input.txt 240 320 1 17 3 3 output.txt

This reads in the pixels from input.txt into a height*width sized array and sets up the row and column counters. There will be two operations, first a blur with kernel 17x17 and then a custom operation with kernel 3x3. The custom operation will ask the user to fill the kernel by hand. When the program finishes, output.txt will contain the new pixel values. 

./main input.txt 240 320 1 17 0 0 output.txt

For only a single operation, use a zero in the second operation.

