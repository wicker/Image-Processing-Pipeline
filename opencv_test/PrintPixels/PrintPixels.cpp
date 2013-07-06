// Written using OpenCV 2.4.5.0 docs as well as 
// http://stackoverflow.com/questions/7899108/opencv-get-pixel-information-from-mat-image
//
// Takes in an image and outputs a text file 'input.png' of grayscale
// values of pixels in a matrix. Text file is useful for being read into
// a program for image processing. 
//
// Jenner Hanni <jeh.wicker@gmail.com>

#include <stdio.h>
#include <iostream>
#include <fstream>
#include <opencv2/opencv.hpp>

using namespace cv;
using namespace std;

int main( int argc, char* argv[] )
{
  if (argc != 4)
  {
    printf( "Usage: ./PrintPixels /path/to/image.png width height\n");
    return -1;
  }

  int width = atoi(argv[2]);
  int height = atoi(argv[3]);

  // this is a grayscaled input image

  Mat input_img(height,width,CV_8UC1);
  input_img = imread(argv[1],0);

  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", input_img );
  waitKey(0);

  // read the matrix out to the input.txt file
  // it's formatted like 134 213 83 68 4  so basically wrapped
  //                     192 23 351 2 123 with space between each

  unsigned char input_array[input_img.rows][input_img.cols];
  ofstream input_file;

  input_file.open ("input.txt");
  for (int rows = 0; rows < input_img.rows; rows++) 
  {
    for (int cols = 0; cols < input_img.cols; cols++) 
    {
      input_file << int(input_img.at<unsigned char>(rows, cols)) << " ";
    }   
  }

  input_file.close();

  printf("\n\n");
  
  return 0;
}

