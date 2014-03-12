// PROGRAM: ImagePixels v1
//
// AUTHOR: Jenner Hanni <jeh.wicker@gmail.com> 
// LICENSE: http://creativecommons.org/licenses/by-sa/4.0/
// DATE: 11-March-2014
// 
// Written using OpenCV 2.4.5.0 docs as well as Stack Overflow: 
// http://stackoverflow.com/questions/7899108/opencv-get-pixel-information-from-mat-image
//
// ImagePixels is a simple tool that turns a color image into a text file
// of grayscale pixels and back into a grayscale image. It's written for
// a program exploring convolution kernels in image processing.
//
// Usage:
//   ./GetPixels.o <operation> <input file> <output file> <width> <height>
//
// <operation>    1 for image -> text file
//                2 for text file -> image
// <image file>   relative path name to an image file
// <pixel file>   relative path name to a text file of pixels
// <width>        width of the image in pixels
// <height>       height of the image in pixels
//
// To get a text file of the pixels of a 800x600 mountain.png, try this:
//   ./GetPixels.o 1 mountain.png pixels.txt 800 600
// To reconstruct an image from that pixel file, try this:
//   ./GetPixels.o 2 mountainReconstructed.png pixels.txt 240 320
//
// Note: mountain.png and mountainReconstructed.png should be identical.
//
// I built the program with OpenCV 2.4.8 and these GCC flags:
//   g++ -o GetPixels.o GetPixels.cpp `pkg-config --cflags --libs opencv` \
//   -L/usr/local/lib -lopencv_core -lopencv_highgui
//

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

#define NUMARGS     6
#define ARGOP       1
#define ARGIMAGE    2
#define ARGPIXELS   3
#define ARGWIDTH    4
#define ARGHEIGHT   5

int main(int argc, char* argv[] )
{
  if (argc != NUMARGS)
  {
    printf( "Usage: ./GetPixels [1 or 2] /path/to/image.png /path/to/pixels.txt width height\n");
    return -1;
  }

  int op = atoi(argv[ARGOP]);
  int width = atoi(argv[ARGWIDTH]);
  int height = atoi(argv[ARGHEIGHT]);
  Mat pixels(height,width,CV_8UC1);
  int temp_pixels[height][width]; 
  ifstream infile;
  ofstream outfile;

  switch(op) {
    case 1:
      printf("You want to turn your image into a text file of pixels.\n");
      // check that the input file exists
      infile.open(argv[ARGIMAGE]);
      if (!infile) {
        cout << ("Cannot open the input image file. Is your path valid?\n");
        return -1;
      }
      infile.close();
      // read in the image pixels into the pixel array
      pixels = imread(argv[ARGIMAGE],0);
      // display the image until the user presses a key
      namedWindow("Display Image", CV_WINDOW_AUTOSIZE);
      imshow("Display Image", pixels);
      waitKey(0);
      // transfer the pixel array to the pixel text file
      outfile.open(argv[ARGPIXELS]);
      for (int rows = 0; rows < pixels.rows; rows++) {
        for (int cols = 0; cols < pixels.cols; cols++) 
          outfile << int(pixels.at<unsigned char>(rows, cols)) << " ";
      }
      outfile.close();
      break;
    case 2:
      printf("You want to turn your text file of pixels into an image.\n");
      // read in the text file pixel of integer values into an array
      infile.open(argv[ARGPIXELS]);
      if (!infile) {
        cout << ("Cannot open the input pixel file. Is your path valid?\n");
        return -1;
      }
      // read in the text file pixel of integer values into an array
      for (int rows = 0; rows < pixels.rows; rows++) {
        for (int cols = 0; cols < pixels.cols; cols++) {
          infile >> temp_pixels[rows][cols];
          pixels.at<unsigned char>(rows,cols) = uint8_t(temp_pixels[rows][cols]);
        }
      }
      infile.close();
      // display the image until the user presses a key
      namedWindow( "Display Image", CV_WINDOW_AUTOSIZE);
      imshow( "Display Image", pixels);
      waitKey(0);
      // create the image in the local folder
      imwrite(argv[ARGIMAGE],pixels);
      break;
    default:
      printf("Operation must be 1 (image->text) or 2 (text->image).");
      return 0;
  }

  return 0;
}

