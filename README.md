Image Processing with Convolution 
=================================

Jenner Hanni  
<jeh.wicker@gmail.com>

Released under the 3-clause BSD License.

OpenCV
------

PrintPixel takes an input PNG and outputs a text file with the grayscale value of the pixels separated by a space. 

164 165 165 165 165 164 165 165 165 164 165 166 

ReconstructImage takes an input text file with the same grayscale pixel values separated by a space and returns an output PNG. 

1. ./PrintPixel input.txt 240 320  
2. use SystemVerilog or C program to modify pixels
3. ./ReconstructImage output.txt 240 320

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
  2 - edge  
  3 - emboss  
  4 - custom  
x1 and y1 are the size N of the kernel NxN

Example:

./main input.txt 240 320 1 17 2 3 output.txt

This reads in the pixels from input.txt into a height*width sized array and sets up the row and column counters. There will be two operations, first a blur with kernel 17x17 and then an edge with kernel 3x3. When the program finishes, output.txt will contain the new pixel values. 

./main input.txt 240 320 1 17 0 0 output.txt

For only a single operation, use a zero in the second operation.

