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
#define ARGNORM   6
#define ARGOUTPUT 7

#define PIXELMIN  0
#define PIXELMAX  255

#define MINHEIGHT 0
#define MAXHEIGHT 900
#define MINWIDTH  0
#define MAXWIDTH  900

#define TRUE      0
#define FALSE     1

#define SUCCESS   0
#define FAIL      1

#define OPMIN     1
#define OP_1DH    1
#define OP_1DV    2
#define OP_CUSTOM 3
#define OPMAX     3

#define CONVMIN   1
#define CONVMAX   17

// parameters includes biggest kernel possible and arguments
// 17*17 + 7 = 289 + 7 = 296 and round up => 300
#define MAXPARAMS 300

#define PARAM_OP          0
#define PARAM_HEIGHT      1
#define PARAM_WIDTH       2
#define PARAM_TOTALPIX    3
#define PARAM_N           4
#define PARAM_TOTALKERNEL 5
#define PARAM_NORM        6
#define PARAM_COEFSTART   7

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
  }

  FILE * fileout;

  // determine if the input file is even valid 
  FILE * filein = fopen(argv[ARGIMAGE],"r");
  if (filein == NULL) {
    printf("  File did not open properly! Does it exist?\n");
    exit_program(FAIL);
  }
  
  // get width and height, then determine if they're in range
  int width = atoi(argv[ARGWIDTH]);
  int height = atoi(argv[ARGHEIGHT]);
  if (width <= MINHEIGHT || width > MAXWIDTH || height <= MINHEIGHT || height > MAXHEIGHT) {
    printf("  Width and height values must be in range 0-900.\n");
    exit_program(FAIL);
  }

  // create inpixels and outpixels arrays from the input height and width
  int inpixels[height*width];
  int outpixels[height*width];

  int reps,i;
  double params[MAXPARAMS];
  int op = atoi(argv[ARGOP1]);
  int N = atoi(argv[ARGSIZE1]);

  // store normalize flag
  int norm = atoi(argv[ARGNORM]);
  if (norm < 0 || norm > 1)
    exit_program(6);


  // print out to the user what we think we're doing this time around
  printf("  The program thinks you want to perform a %dx%d kernel operation on a %dx%d image.\n",N,N,width,height);
  
  // error check the inputs for each operation and return errors to user if invalid
  if (op < OPMIN || op > OPMAX)
    exit_program(2);
  else if (N %2 == 0)
    exit_program(3);
  else if (N < CONVMIN || N > CONVMAX)
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
  params[PARAM_NORM] = norm;

  // read input pixels 
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
    printf("  File did not open properly! Does it exist?\n");
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
  int line = (int)params[PARAM_N];
  double horz[line];
  double vert[line];

  // 1D horizontal or vertical coefficients
  if (op == OP_1DH || op == OP_1DV) {
    // poll the user appropriately
    if (op == OP_1DH) printf("  Please enter the %d horizontal coefficients, separated by a space or enter.\n",line);
    else              printf("  Please enter the %d vertical coefficients, separated by a space or enter.\n",line);
    // in either case, fill the params the same way
    for (i = 0; i < params[PARAM_N]; i++) {
      scanf("%lf",&params[PARAM_COEFSTART+i]);
    }
  }
  // custom coeffs 
  else if (op == OP_CUSTOM) {
    printf("  Please enter the coefficients for your custom operation\n");
    printf("  from left to right with an enter after each.\n",params[PARAM_TOTALKERNEL]);
    for (i = PARAM_COEFSTART; i < PARAM_COEFSTART+params[PARAM_TOTALKERNEL]; i++) 
      scanf("%lf",&params[i]);
  }
}

// convolution()
// uses the given params array to iterate through each image pixel
// and calculate a new output based on the pixel's nearby neighbors
void convolution(int *inpixels,double *params,int *outpixels) {

  double t = 0;
  int i,irow,icol,krow,kcol,kx,ky,kcount,discardpix,r,k;
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
      k = 0;

      for (kcount = PARAM_COEFSTART,krow = 1,kx = -krows/2; krow <= krows; krow++,kx++) {
        for (kcol = 1,ky = -kcols/2; kcol <= kcols; kcol++,kcount++,ky++) {
          if (kx + irow <= 0 || ky + icol <= 0 || kx + irow > irows || ky + icol > icols) {
            discardpix++;
            printf("EDGE CASE irow %d icol %d kx %d ky %d\n",irow,icol,kx,ky);
            r++;
          }
          else
            if (params[PARAM_OP] == OP_1DH) {  // 1D horizontal operation 
                if (kx == 0) {
                  t += params[PARAM_COEFSTART+k]*inpixels[i+kx*icols+ky];
                  k++;
                }
                else
                  discardpix++;
            }
            else if (params[PARAM_OP] == OP_1DV) {  // 1D vertical operation 
                if (ky == 0) {
                  t += params[PARAM_COEFSTART+k]*inpixels[i+kx*icols+ky];
                  k++;
                }
                else
                  discardpix++;
            }
            else {  // otherwise it's a full kernel convolution 
              t += params[PARAM_COEFSTART+r]*inpixels[i+kx*icols+ky];
              printf("ACTUAL PIXEL t %lf r %d i %d kx %d icols %d ky %d\n",t,r,i,kx,icols,ky);
              r++;
          }
        }
      }

      // check if we need to normalize (average) the image
      if (params[PARAM_NORM] == FALSE) {
        t = t/(params[PARAM_TOTALKERNEL]-discardpix);
      }
      else  {
        t = t;
      }

      // bound the pixel at the min and max (for grayscale this is 0 and 255)
      if (t < PIXELMIN) 
        outpixels[i] = t;
      else if (t > PIXELMAX)
        outpixels[i] = PIXELMAX;
      else
        outpixels[i] = t;
      printf("%lf\n",t);
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
    case 6:
      printf("  Normalize value must be 0 for no or 1 for yes. ");
    default:
      break;
  }
  printf("  Program will now exit.\n\n");
  printf("======================================================================================================\n\n");
  exit(0);
}
