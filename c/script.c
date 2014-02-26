// Image convolution
// Jenner Hanni <jeh.wicker.gmail.com>
// 3-clause BSD license
//
// Script version that takes more inputs from the command line
// Allows custom output file name.
// Does not prompt the user, just takes inputs.

#define ARGS 9

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

  // check that we received the correct number of arguments
  if (argc != 9) {
    printf("Input should look like this:\n");
    printf("./script input/input-brokentop.txt 240 320 1 17 2 3 output.txt\n");
    exit_program(1);
    return -1;
  }

  FILE * fileout;

  // determine if the input file is even valid 
  FILE * filein = fopen(argv[1],"r");
  if (filein == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(1);
    return -1;
  }
  
  // get width and height, then determine if they're in range
  int width = atoi(argv[2]);
  int height = atoi(argv[3]); 
  if (width <= 0 || width > 900 || height <= 0 || height > 900) {
    printf("Width and height values must be in range 0-900.\n");
    exit_program(1);
    return -1;
  }

  // create inpixels and outpixels arrays from the input height and width
  int inpixels[height*width];
  int outpixels[height*width];

  int reps;
  int firstop = atoi(argv[4]);
  int firstN = atoi(argv[5]);
  int secondop = atoi(argv[6]);
  int secondN = atoi(argv[7]);
  int i,params[300];

  // check if either kernel size argument or convolution operation is zero
  if (firstop == 0 || firstop < 1 || firstop > 3) {
    printf("Empty or illegal entry for first operation choice. Checking second operation.\n");
    if (secondop == 0 || secondop < 1 || secondop > 3) {
      printf("Illegal choice for second operation. Review the instructions. ");
      exit_program(1);
      return(-1);
    }
    else if (secondN % 2 == 0) {
      printf("Kernel matrix N in NxN must be an odd number. Try again.\n");
      exit_program(1);
      return(-1);
    }
    else if (secondN < 1 || secondN > 17) {
      printf("N in kernel matrix NxN must be between 1 and 17.\n");
      exit_program(1);
      return(-1);
    }
    // go ahead and run the second operation
    else {
      printf("Running the second operation.\n");
      params[0] = secondop;
      params[1] = height;
      params[2] = width;
      params[3] = height*width;

      // determine if input file is input.txt or output.txt
      if (firstop == 0 || firstop < 1 || firstop > 3)
        filein = fopen(argv[8],"r");
        if (filein == NULL) {
          printf("Either input or output file did not open properly! Does it exist?\n");
          exit_program(1);
          return -1;
        }
      else
        filein = fopen(argv[1],"r");

      // read in the input file
      for (i = 0; i < params[3]; i++) 
        fscanf(filein, "%d ", &inpixels[i]);
      fclose(filein);

      params[4] = secondN;
      params[5] = secondN*secondN;

      if (params[0] == 1) {
        for (i = 6; i < 6+params[5]; i++)
          params[i] = 1;
      }
      else {
        printf("Only blur operation is implemented. Please enter the coefficients for your custom operation.\n");
        printf("Enter all %d coefficients in order from left to right with an enter after each.\n",params[5]);
        for (i = 6; i < 6+params[5]; i++) 
          scanf("%d",&params[i]);
      }
      
      // use the parameters to perform the convolution
      convolution(inpixels,params,outpixels);

      // save the output to the output text file
      fileout = fopen("output.txt","w");
      if (fileout == NULL) {
        printf("File did not open properly! Does it exist?\n");
        exit_program(1);
        return 0;
      }  
      for (i = 0; i < params[3]; i++) 
        fprintf(fileout, "%d ", outpixels[i]);
      fclose(fileout);

    } // end running of the second operation

    exit_program(0);
    return 0;
   
  }

  // first op is legal, perform the first op and then the second op
  else {
    params[0] = firstop;
    params[1] = height;
    params[2] = width;
    params[3] = height*width;

    // the input file is input.txt because this is the first operation
    filein = fopen(argv[1],"r");
    if (filein == NULL) {
      printf("Either input or output file did not open properly! Does it exist?\n");
      exit_program(1);
      return -1;
    }

    // read in the input file
    for (i = 0; i < params[3]; i++) 
      fscanf(filein, "%d ", &inpixels[i]);
    fclose(filein);

    params[4] = firstN;
    params[5] = firstN*firstN;

    if (params[0] == 1) {
      for (i = 6; i < 6+params[5]; i++)
        params[i] = 1;
    }
    else {
      printf("Only blur operation is implemented. Please enter the coefficients for your custom operation.\n");
      printf("Enter all %d coefficients in order from left to right with an enter after each.\n",params[5]);
      for (i = 6; i < 6+params[5]; i++) 
        scanf("%d",&params[i]);
    }
    
    // use the parameters to perform the convolution
    convolution(inpixels,params,outpixels);

    // save the output to the output text file
    fileout = fopen("output.txt","w");
    if (fileout == NULL) {
      printf("File did not open properly! Does it exist?\n");
      exit_program(1);
      return 0;
    }  
    for (i = 0; i < params[3]; i++) 
      fprintf(fileout, "%d ", outpixels[i]);
    fclose(fileout);

    // now determine if the second operation is valid and perform it if it is
    
    printf("First operation complete. Checking for a second operation.\n");
    if (secondop == 0) {
      printf("No second operation.\n");
      exit_program(0);
    }      
    else if (secondop < 1 || secondop > 3) {
      printf("Illegal choice for second operation. Review the instructions. ");
      exit_program(1);
      return(-1);
    }
    else if (secondN % 2 == 0) {
      printf("Second operation unable to finish.\n");
      printf("Kernel matrix N in NxN must be an odd number. Try again.\n");
      exit_program(1);
      return(-1);
    }
    else if (secondN < 1 || secondN > 17) {
      printf("N in kernel matrix NxN must be between 1 and 17.\n");
      exit_program(1);
      return(-1);
    }
    // go ahead and run the second operation
    else {
      printf("Running the second operation.\n");
      params[0] = secondop;
      params[1] = height;
      params[2] = width;
      params[3] = height*width;

      // the input file is going to be output.txt after first operation
      filein = fopen(argv[8],"r");
      if (filein == NULL) {
        printf("Either input or output file did not open properly! Does it exist?\n");
        exit_program(1);
        return -1;
      }

      // read in the input file
      for (i = 0; i < params[3]; i++) 
        fscanf(filein, "%d ", &inpixels[i]);
      fclose(filein);

      params[4] = secondN;
      params[5] = secondN*secondN;

      if (params[0] == 1) {
        for (i = 6; i < 6+params[5]; i++)
          params[i] = 1;
      }
      else {
        printf("Only blur operation is implemented. Please enter the coefficients for your custom operation.\n");
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

    } // end running of the second operation

    exit_program(0);
    return 0;
  }

  printf("not sure how you got here.");

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
  return 0;
}
