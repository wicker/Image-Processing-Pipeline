// Image convolution
// Jenner Hanni <jeh.wicker.gmail.com>
// 3-clause BSD license

#include <stdio.h>
#include <stdlib.h>

int exit_program(int);
void convolution(int *inpixels,int *params,int *outpixels);

// main()
// 1. receives args and error checks inputs
// 2. opens the text file and saves input pixels
// 3. polls user for convolution type and kernel size
// 4. performs the convolution
// 5. outputs the pixels to a text file
// 6. uses the output text file as an input and repeats steps 2-5 
//    if more than one operation in a row

int main(int argc, char *argv[]) {

  int height,width,yn,i; 

  // determine if the input file is even valid 
  FILE * filein = fopen(argv[1],"r");
  if (filein == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(1);
    return 0;
  }
  
  // poll user for the image height and width
  printf("Enter the image width: ");
  scanf("%d",&width);
  printf("Enter the image height: ");
  scanf("%d",&height);
  printf("You entered an image size of width %d and height %d.",width,height);
  printf("Enter a '0' if this is true. ");
  scanf("%d",&yn);
  if (yn) {
    exit_program(1);
    return 0;
  }

  // create inpixels and outpixels arrays from the input height and width
  int inpixels[height*width];
  int outpixels[height*width];

  // poll the user for the convolution parameters
  
  int reps;

  // determine how many operations to perform on that image
  printf("Do you want to perform 1 or 2 convolution operations on this image? ");
  scanf("%d",&reps);

  if (reps < 1 || reps > 2) {
    printf("You wanted to perform %d successive operations on your image. \n",reps);
    printf("The program only supports 1 or 2 operations.\n");
    exit_program(1);
    return 0;
  }
  
  if (reps == 1)
    printf("There will be %d operation performed on this image.\n",reps);
  else
    printf("There will be %d operations performed on this image.\n",reps);

  // for each operation in the sequence, collect the user's information
  // and perform the convolution, then save the output to a file.

  int rep, params[300];
  params[0] = 0;              // desired convolution operation
  params[1] = height;         // number of image rows
  params[2] = width;          // number of image columns
  params[3] = height*width;   // number of image pixels

  for (rep = 1; rep <= reps; rep++) {

    // if this is the first rep, fill the inpixels array with data from input file
    if (rep == 1) {
      for (i = 0; i < params[3]; i++) 
        fscanf(filein, "%d ", &inpixels[i]);
      fclose(filein);
    }

    // if this isn't the first rep, it needs the previous saved output pixels
    else {
      filein = fopen("output.txt","r");
      for (i = 0; i < params[3]; i++) {
        fscanf(filein, "%d ", &inpixels[i]);
      }
      fclose(filein);
    }

    // get kernel size and convolution coefficients from the user
    printf("\n======== OPERATION #%d ==========\n",rep);
    printf("For the kernel matrix of size NxN, enter the desired integer value of N.\n");
    printf("This program supports odd values between 1 and 17. Enter a value:  ");
    scanf("%d",&params[4]);

    while (params[4] % 2 == 0 || params[4] > 17 || params[4] < 0) { 
      printf("N must be an odd number between 1 and 17. Choose again: ");
      scanf("%d",&params[4]);
    } 

    params[5] = params[4]*params[4];
    printf("Which convolution operation? You can choose blur (1) or custom (2). ");
    scanf("%d",&params[0]);
    if (params[0] < 1 || params[0] > 2) {
      printf("Illegal input. Try again: ");
    }
    if (params[0] == 1) {
      for (i = 6; i < 6+params[5]; i++)
        params[i] = 1;
    }
    else {
      printf("Please enter the coefficients for a custom operation.\n");
      printf("Enter all %d coefficients in order from left to right with an enter after each.\n",params[5]);
      for (i = 6; i < 6+params[5]; i++) 
        scanf("%d",&params[i]);
      
    }

    // use the parameters to perform the convolution
    convolution(inpixels,params,outpixels);

    // save the output to the output text file
    FILE * fileout = fopen("output.txt","w");
    if (fileout == NULL) {
      printf("File did not open properly! Does it exist?\n");
      exit_program(1);
      return 0;
    }  
    for (i = 0; i < params[3]; i++) 
      fprintf(fileout, "%d ", outpixels[i]);
    fclose(fileout);
  }

  // report success and close the program
  exit_program(0);
  return 0;
}

// convolution()
// uses the given params array to iterate through each image pixel
// and calculate a new output based on the pixel's nearby neighbors
void convolution(int *inpixels,int *params,int *outpixels) {

  int t = 0;
  int i,irow,icol,krow,kcol,kx,ky,kcount,discardpix;
  int irows = params[1];
  int icols = params[2]; 
  int krows = params[4];
  int kcols = params[4];

  // first, orient the source image's pixel of interest with irow and icol
  // then use the params array which contains the kernel matrix coefficients to the
  // image pixels around our image pixel to calculate output value
  for (i = 0,irow = 1; irow <= irows; irow++) {
    for (icol = 1; icol <= icols; icol++,i++) {

      // reset the output for this pixel
      t = 0; 
      discardpix = 0;

      for (kcount = 6,krow = 1,kx = -kcols/2; krow <= krows; krow++,kx++) {
        for (kcol = 1,ky = -kcols/2; kcol <= kcols; kcol++,kcount++,ky++) {
          if (kx + irow <= 0 || kx + icol <= 0 || kx + irow > irows || ky + icol > icols)
            discardpix++;
          else
              t += params[kcount]*inpixels[i+kx*icols+ky];
        }
      }

      // get the average of the summed result of the kernel coefficients over the input pixels
      // discardpix is the number of pixels that were off the edge of the image and unusable
      outpixels[i] = t/(params[5]-discardpix);
    }
  }
}

int exit_program(int result) {
  if (result == 1)
    printf("The program will now exit. Please try again.\n");
  if (result == 0)
    printf("Program successfully completed.\n");
    printf("======================================\n");
  return 0;
}
