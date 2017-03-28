Image Processing with Convolution 
=================================

Jenner Hanni  
<jeh.wicker@gmail.com>

Released under the 3-clause BSD License.

<img src="https://jenner.smugmug.com/Convolution-and-GetPixels/n-3qNVXF/i-7RBP58r/0/O/i-7RBP58r.png">

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

The convolution program works by shoving an image's pixel values into an array and iterating over them with a convolution kernel matrix. The program takes arguments to specify input and output file names along with the number of kernels (1 or 2), kernel size (odd numbers between 1 and 17), and kernel coefficients (integers).

Usage:  `./main.o <input.txt> <width> <height> <op> <kernel> <norm> <output.txt>`

<code>input.txt</code> is the text file containing grayscale image pixel values.  

<code>width</code> in pixels, same as number of columns in image.  

<code>height</code> in pixels, same as number of rows in image.  

<code>op</code> is (1) a horizontal 1d, (2) a vertical 1d, or (3) custom kernel to be entered by the user.  

<code>kernel</code> is the N in the NxN kernel matrix that must be an odd number between 1 and 17.  

<code>norm</code> is (0) no or (1) yes to normalize (divide by number of coefficients) the image.  

<code>output.txt</code> is the text file to store resulting grayscale image pixel values.  

To perform a custom 2D operation on a 240x320 image with a 5x5 filter kernel that will be normalized:

<code>./main.o input.txt 240 320 3 5 1 output.txt</code>

To perform a 1D horizontal operation on the same 240x320 image with a 3x3 filter kernel that will not be normalized:

<code>./main.o input.txt 240 320 1 3 0 output.txt</code>

<h5>Scripting Example</h5>

I put the following in a script to run a bunch of kernels all in a row. I used the same input image (brokentop.txt) but saved the outputs to their own files so I could reconstruct them with GetPixels at the end. The image is a portrait size of 240x320. I chose to do a single custom 3x3 operation and none of them are to be normalized.

First, I turned the brokentop.png image into a text file.

<code>./GetPixels.o 1 brokentop.png brokentop.txt 240 320</code>

Then I performed the operations, creating six output text files with the results.

```
./main.o brokentop.txt 240 320 3 3 0 output-horz.txt
./main.o brokentop.txt 240 320 3 3 0 output-sobel-h.txt  
./main.o brokentop.txt 240 320 3 3 0 output-vert.txt 
./main.o brokentop.txt 240 320 3 3 0 output-sobel-v.txt 
./main.o brokentop.txt 240 320 3 3 0 output-45deg.txt 
./main.o brokentop.txt 240 320 3 3 0 output-135deg.txt 
```

Finally, I reconstructed images from the output files so I could look at them in an image viewer.

```
./GetPixels.o 2 output-horz.png output-horz.txt 240 320
./GetPixels.o 2 output-sobel-h.png output-sobel-h.txt 240 320
./GetPixels.o 2 output-vert.png output-vert.txt 240 320
./GetPixels.o 2 output-sobel-v.png output-sobel-v.txt 240 320
./GetPixels.o 2 output-45deg.png output-45deg.txt 240 320
./GetPixels.o 2 output-135deg.png output-135deg.txt 240 320
```

For each operation, the program prompted me to enter the coefficients from left to right by hand but I have a couple options for how to enter them. Using the kernel for a Sobel horizontal operator, I can enter the coefficients all on one line like this,

`+1 +2 +1 0 0 0 -1 -2 -1`

or I can enter them by lines like this,

```
+1 +2 +1
0 0 0 
-1 -2 -1
```

which can be easier to read. Finally, I can enter them all like this,

```
+1 
+2   
+1 
0 
0 
0 
-1 
-2 
-1
```

so it really doesn't matter. I ended up writing them in the second form in a text file and copying/pasting. That way the test is built before I start and the results come out without needing editing. 
