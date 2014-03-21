// Image convolution
// Jenner Hanni <jeh.wicker.gmail.com>
// 3-clause BSD license
//
// This is a single-operation version.
// The script can be used to run the program multiple times

#include <stdio.h>
#include <stdlib.h>
#include <math.h>

#define NUMARGS   8
#define ARGIMAGE  1
#define ARGWIDTH  2
#define ARGHEIGHT 3
#define ARGOP1    4
#define ARGSIZE1  5
#define ARGOUTPUT 6

#define PIXELMIN 0
#define PIXELMAX 255

#define TRUE 0
#define FALSE 1

#define SUCCESS 0
#define FAIL 1

// parameters includes biggest kernel possible and arguments
// 17*17 + 7 = 289 + 7 = 296 and round up => 300
#define MAXPARAMS 300

#define PARAM_OP          0
#define PARAM_HEIGHT      1
#define PARAM_WIDTH       2
#define PARAM_TOTALPIX    3
#define PARAM_N           4
#define PARAM_TOTALKERNEL 5
#define PARAM_COEFSTART   6

int exit_program(int);
void convolution(int *inpixels,double *params,int *outpixels);
void fill_kernel(int op, double *params);

// main()
// 1. receives args and error checks inputs
// 2. opens the text file and saves input pixels
// 3. polls user for convolution type and kernel size
// 4. performs the convolution
// 5. outputs the pixels to a text file
// 6. uses the output text file as an input and repeats steps 2-5 
//    if more than one operation in a row

int main(int argc, char *argv[]) {

  printf("\n======================================================================================================\n");
  printf("                   1D and 2D Image Convolution                  \n");
  printf("======================================================================================================\n");


  // check that we received the correct number of arguments
  if (argc != NUMARGS) {
    printf("\n  Incorrect number of arguments!\n  Input should look like this:\n\n  ./script input.txt <width> <height> <op> <kernel> <norm> output.txt\n\n");
    printf("  <input.txt> is the text file containing grayscale image pixel values.\n");
    printf("  <width> in pixels, same as number of columns in image.\n");
    printf("  <height> in pixels, same as number of rows in image.\n");
    printf("  <op> is [1] a horizontal 1D, [2] a vertical 1D, or [3] custom kernel to be entered by the user.\n");
    printf("  <op> is [1] a horizontal 1D, [2] a vertical 1D, or [3] custom kernel to be entered by the user.\n");
    printf("  <kernel> is the N in NxN that must be an odd number between 1 and 17.\n");
    printf("  <norm> is [0] no or [1] yes to normalize (divide by number of coefficients) the image.\n");
    printf("  <output.txt> is the text file to store resulting grayscale image pixel values.\n\n");
    exit_program(5);
    return -1;
  }
  else {
    printf("  The program thinks you want to perform a %dx%d kernel operation on a %dx%d image.\n");
  }

  FILE * fileout;

  // determine if the input file is even valid 
  FILE * filein = fopen(argv[ARGIMAGE],"r");
  if (filein == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(FAIL);
    return -1;
  }
  
  // get width and height, then determine if they're in range
  int width = atoi(argv[ARGWIDTH]);
  int height = atoi(argv[ARGHEIGHT]);
  if (width <= 0 || width > 900 || height <= 0 || height > 900) {
    printf("Width and height values must be in range 0-900.\n");
    exit_program(FAIL);
    return -1;
  }

  // create inpixels and outpixels arrays from the input height and width
  int inpixels[height*width];
  int outpixels[height*width];

  int reps,i;
  double params[MAXPARAMS];
  int op = atoi(argv[ARGOP1]);
  int N = atoi(argv[ARGSIZE1]);

  // error check the inputs for each operation and return errors to user if invalid
  if (op < 1 || op > 4)
    exit_program(2);
  else if (N %2 == 0)
    exit_program(3);
  else if (N < 1 || N > 17)
    exit_program(4);

  // prep params with the operation inputs, including reading appropriate input file
  // then perform the convolution and produce the output file
  //
  // first, store parameter values
  params[PARAM_OP] = op;
  params[PARAM_HEIGHT] = height;
  params[PARAM_WIDTH] = width;
  params[PARAM_TOTALPIX] = height*width;
  params[PARAM_N] = N;
  params[PARAM_TOTALKERNEL] = N*N;

  // read in input pixels 
  filein = fopen(argv[ARGIMAGE],"r");
  for (i = 0; i < params[PARAM_TOTALPIX]; i++) 
    fscanf(filein, "%d ", &inpixels[i]);
  fclose(filein);

  // read in coefficients from user and perform convolution
  fill_kernel(params[PARAM_OP],params);
  convolution(inpixels,params,outpixels);

  // prep output file
  fileout = fopen(argv[ARGOUTPUT],"w");
  if (fileout == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(FAIL);
  }  
  for (i = 0; i < params[PARAM_TOTALPIX]; i++) 
    fprintf(fileout, "%d ", outpixels[i]);
  fclose(fileout);

  return 0;
}

// fill kernel()
// description
void fill_kernel(int op, double *params) {

  int i;
  int v = 0;
  int h = 0;
  int line = (int)params[4];
  double horz[line];
  double vert[line];

  // uniform blur
  if (op == 1) {
    for (i = 6; i < 6+params[5]; i++)
      params[i] = 1;
  }
  // 1D horizontal or vertical coefficients
  else if (op == 2 || op == 3) {
    // poll the user appropriately
    if (op == 2)
      printf("Please enter the %d horizontal coefficients, separated by a space or enter.\n",line);
    else if (op == 3)
      printf("Please enter the %d vertical coefficients, separated by a space or enter.\n",line);
    // in either case, fill the params the same way
    for (i = 0; i < params[4]; i++) {
      scanf("%lf",&params[6+i]);
    }
  }
  // custom coeffs 
  else if (op == 4) {
    printf("Please enter the coefficients for your custom operation\n");
    printf("from left to right with an enter after each.\n",params[5]);
    for (i = 6; i < 6+params[5]; i++) 
      scanf("%lf",&params[i]);
  }
}

// convolution()
// uses the given params array to iterate through each image pixel
// and calculate a new output based on the pixel's nearby neighbors
void convolution(int *inpixels,double *params,int *outpixels) {

  double t = 0;
  int i,irow,icol,krow,kcol,kx,ky,kcount,discardpix,r;
  int irows = params[PARAM_HEIGHT];
  int icols = params[PARAM_WIDTH]; 
  int krows = params[PARAM_N];
  int kcols = params[PARAM_N];

  // first, orient the source image's pixel of interest with irow and icol
  // then use the params array which contains the kernel matrix coefficients to the
  // image pixels around our image pixel to calculate output value
  for (i = 0,irow = 1; irow <= irows; irow++) {
    for (icol = 1; icol <= icols; icol++,i++) {

      // reset the output for this pixel
      t = 0; 
      discardpix = 0;
      r = 0;

      for (kcount = PARAM_COEFSTART,krow = 1,kx = -kcols/2; krow <= krows; krow++,kx++) {
        for (kcol = 1,ky = -kcols/2; kcol <= kcols; kcol++,kcount++,ky++) {

          if (kx + irow <= 0 || ky + icol <= 0 || kx + irow > irows || ky + icol > icols) {
            discardpix++;
//            r++;
          }
          else
            if (params[PARAM_OP] == 2) {  // 1D horizontal operation 
                if (kx == 0) {
                  t += params[PARAM_COEFSTART+r]*inpixels[i+kx*icols+ky];
                  r++;
                }
                else
                  discardpix++;
            }
            else if (params[PARAM_OP] == 3) {  // 1D vertical operation 
                if (ky == 0) {
                  t += params[PARAM_COEFSTART+r]*inpixels[i+kx*icols+ky];
                  r++;
                }
                else
                  discardpix++;
            }
            else {  // otherwise it's a full kernel convolution 
              t += params[PARAM_COEFSTART+r]*inpixels[i+kx*icols+ky];
              r++;
            }
        }
      }

      // if this is a uniform blur with coefficients = 1
      // get the average of the summed result of the kernel coefficients over the input pixels
      // discardpix is the number of pixels that were off the edge of the image and unusable
      if (params[PARAM_OP] == 1 || params[PARAM_OP] == 2 || params[PARAM_OP] == 3)
        t = t/(params[PARAM_TOTALKERNEL]-discardpix);
      else 
        t = t;

      // bound the pixel at the min and max (for grayscale this is 0 and 255)
      if (t < PIXELMIN) 
        outpixels[i] = t;
      else if (t > PIXELMAX)
        outpixels[i] = PIXELMAX;
      else
        outpixels[i] = t;
    }
  }
}

int exit_program(int result) {
  printf("======================================================================================================\n\n");
  printf("  Results: ");
  switch (result) {
    case 0:
      printf("  Program successfully completed.\n"); 
      break;
    case 1:
      printf("  Fail! ");
      break;
    case 2:
      printf("  Empty or illegal entry for operation choice.\n");
      break;
    case 3:
      printf("  Kernel matrix N in NxN must be an odd number.\n");
      break;
    case 4:
      printf("  N in kernel matrix NxN must be between 1 and 17.\n");
      break;
    case 5:
      printf("  Try again. ");
    default:
      break;
  }
  printf("  Program will now exit.\n\n");
  printf("======================================================================================================\n\n");
  exit(0);
}
