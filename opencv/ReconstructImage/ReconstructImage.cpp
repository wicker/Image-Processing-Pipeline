// Written using OpenCV docs
//
// Takes in a text file of 8-bit grayscale pixels along with 
// arguments of height and width. Renders the contents on screen
// and outputs to an image file.
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
    printf( "Usage: ./ReconstructImage /path/to/pixel-file.txt width height\n");
    return -1;
  }

  int width = atoi(argv[2]);
  int height = atoi(argv[3]);

  // this structure will hold the reconstructed image
  
  Mat output_img(height,width,CV_8UC1);

  // up to this point input.txt is beautiful
  // now read in input.txt into the output_array
  // and print output_array contents

  int output_array[output_img.rows][output_img.cols];
  ifstream output_file;
  int foo;

  output_file.open ( argv[1]);

  if (!output_file) {
    cout << "Cannot open the file.\n";
  }

  for (int rows = 0; rows < output_img.rows; rows++) 
  {
    for (int cols = 0; cols < output_img.cols; cols++) 
    {
      output_file >> output_array[rows][cols];
      output_img.at<unsigned char>(rows,cols) = uint8_t(output_array[rows][cols]);
    } 
  }

  printf("\n\n");
  output_file.close();
  
  // now it should be the original grayscale image

  namedWindow( "Display Image", CV_WINDOW_AUTOSIZE );
  imshow( "Display Image", output_img );
  waitKey(0);

  // create the modified PNG in the local folder
  imwrite("output.png", output_img);

  return 0;
}
