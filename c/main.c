// Image convolution
// Jenner Hanni <jeh.wicker.gmail.com>
// 3-clause BSD license
//
// todo: support presets 2 = edge, 3 = emboss for any size kernel
//
// todo: make it so in kernels bigger than 3x3 the outer pixels are propagated
// out but the intermediate ones between poi and the edge are convoluted
// properly.
//
// todo: make it play nice with scripting

#include <stdio.h>
#include <stdlib.h>

int instructions();
int exit_program(int);

//handle_meta()
//1. uses the name of the file and image size to fill the pixel array
//2. error checks the user input
int handle_input_meta(int *inpixels);

// handle_params()
// 1. polls user and fills params array with num and type of ops, kernel size, and image size
// 2. use kernel size to fill coeffs array as appropriate
// 3. error checks the user input
int handle_input_params(int *params1,int *params2);

// convolution()
// 1. uses the given params array to 
int convolution(int *inpixels,int *params,int *outpixels);

// main()
// 1. receives args and error checks inputs
// 2. calls handle_input() to receive all the convolution parameters
int main(int argc, char *argv[]) {

  int height, width,yn; 

  // handle_meta()
  // poll user for the image height and width
  printf("Enter the image width: ");
  scanf("%d",&width);
  printf("Enter the image height: ");
  scanf("%d",&height);
  printf("You entered an image size of width %d and height %d. Enter a '0' if this is true. ",width,height);
  scanf("%d",&yn);
  if (yn) {
    exit_program(1);
    return 0;
  }

  // save the name of the file to operate on
  FILE * filein = fopen(argv[1],"r");
  if (filein == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(1);
    return 0;
  }
  
  // create inpixels and outpixels arrays from the input height and width
  int inpixels[height*width];
  int outpixels1[height*width];
  int outpixels2[height*width];

  // fill the inpixels array with data from the input file
  int size = height*width;
  int size1 = 1;
  int size2 = 1;
  int rows = height;
  int cols = width;
  int i;
  for (i = 0; i < size; i++) {
    fscanf(filein, "%d ", &inpixels[i]);
    printf("%d ",inpixels[i]);
  }
  printf("\n");

  // poll the user for the convolution parameters
  
  int kernelsize,reps,first_op,second_op,row,col;

  // determine how many operations to perform on that image
  printf("Do you want to perform 1 or 2 convolution operations on this image? ");
  scanf("%d",&reps);

  if (reps < 1 || reps > 2) {
    printf("You wanted to perform %d successive operations on your image.\n",reps);
    printf("The program only supports 1 or 2 operations.\n");
    exit_program(1);
    return 0;
  }
  
  if (reps == 1)
    printf("There will be %d operation performed on this image.\n",reps);
  else
    printf("There will be %d operations performed on this image.\n",reps);

  // create the two parameters arrays and set the desired operation flag to zero
  int params2[300];
  int params1[300];
  params1[0] = params2[0] = 0;              // desired convolution operation
  params1[1] = params2[1] = height;         // number of image rows
  params1[2] = params2[2] = width;          // number of image columns
  params1[3] = params2[3] = height*width;   // number of image pixels 

  while (reps > 0) {
    // determine the size of the convolution kernel matrix
    
    printf("For the kernel matrix of size NxN, enter the desired integer value of N. ");

    scanf("%d",&kernelsize);
    if (kernelsize % 2 == 0) {
      printf("The kernel must be an odd number.\n");
      exit_program(1);
      return 0;
    }

    if (reps == 2) {
      params2[4] = kernelsize;              // value of N in NxN kernel matrix
      params2[5] = kernelsize*kernelsize;   // number of kernel coefficients
      printf("Which operation for the second operation? Blur (1) or Custom (2) ");
      scanf("%d",&params2[0]);
      if (params2[0] < 1 || params2[0] > 2) {
        printf("Please choose 1 or 2: ");
        scanf("%d",&params2[0]);
      }
      if (params2[0] < 1 || params2[0] > 2) {
        printf("The program will now close.\n");
        exit_program(1);
      }
      switch(params2[0]) {
        case 1:
          printf("You've selected a blur operation.\n");
          for (i = 6; i < 6+params2[5]; i++)
            params2[i] = 1;
          break;
        case 2:
        case 3:
        case 4:
        default:
          printf("Please enter the coefficients for a custom operation.\n");
          printf("Enter all %d coefficients in order from left to right with an enter after each.\n",params2[5]);
          for (i = 6; i < 6+params2[5]; i++) 
            scanf("%d",&params2[i]);
      }
    }
    else if (reps == 1) {
      params1[4] = kernelsize;              // value of N in NxN kernel matrix
      params1[5] = kernelsize*kernelsize;   // number of kernel coefficients
      printf("Which operation for the first operation? Blur (1) or Custom (2) ");
      scanf("%d",&params1[0]);
      if (params1[0] < 1 || params1[0] > 2) {
        printf("Please choose 1 or 2: ");
        scanf("%d",&params1[0]);
      }
      if (params1[0] < 1 || params1[0] > 2) {
        printf("The program will now close.\n");
        exit_program(1);
      }
      switch(params1[0]) {
        case 1:
          printf("You've selected a blur operation.\n");
          for (i = 6; i < 6+params1[5]; i++)
            params1[i] = 1;
          break;
        case 2:
        case 3:
        case 4:
        default:
          printf("Please enter the coefficients for a custom operation.\n");
          printf("Enter all %d coefficients in order from left to right with an enter after each.\n",params2[5]);
          for (i = 6; i < 6+params1[5]; i++) 
            scanf("%d",&params1[i]);
      }
    }

    reps--;
  }

  // create the output by performing the convolution operation on each kernel
  // start with kernel #1
  if (params1[0] != 0) {
    convolution(inpixels,params1,outpixels1);
    FILE * fileout1 = fopen("output1.txt","w");
    if (fileout1 == NULL) {
      printf("File did not open properly! Does it exist?\n");
      exit_program(1);
      return 0;
    }  
    for (i = 0; i < params1[3]; i++) 
      fprintf(fileout1, "%d ", outpixels1[i]);
    fclose(fileout1);
  }

  // and now kernel #2 if we chose to do it
  if (params2[0] != 0) {
    convolution(outpixels1,params2,outpixels2);

    FILE * fileout2 = fopen("output2.txt","w");
    if (fileout2 == NULL) {
      printf("File did not open properly! Does it exist?\n");
      exit_program(1);
      return 0;
    }  
    for (i = 0; i < params2[3]; i++) 
      fprintf(fileout2, "%d ", outpixels2[i]);
  }

  exit_program(0);
  return 0;

}

int convolution(int *inpixels,int *params,int *outpixels) {

  int t = 0;
  int i,row,col,m,n,x,y,k;
  int rows = params[1];
  int cols = params[2]; 
  int krow = params[4];
  int kcol = params[4];

  printf("rows: %d\tcols: %d\n",rows,cols);

  // first, orient the pixel of interest with rows and cols
  for (i = 0,row = 1; row <= rows; row++) {
    for (col = 1; col <= cols; col++,i++) {

      // second, use the params to use the pixels around it to calculate output pixel
      if (row == 1 || row == rows || col == 1 || col == cols) { 
        t = inpixels[i];
        printf("%d = inpixels[%d]\n",t,i);
      }
      else { // perform the convolution on that pixel
        t = 0;
        
        for (k = 6,m = 0,x = -kcol/2; m < krow; m++,x++) {
          for (n = 0,y = -kcol/2; n < kcol; n++,k++,y++) {
            t += params[k]*inpixels[i-x*cols-y];
          }
        }
        if (params[0] == 1) 
          t = t/params[5];
      }
      outpixels[i] = t;
    }
  }
  return 0;  
}

int exit_program(int result) {
  if (result == 1)
    printf("The program will now exit. Please try again.\n");
  if (result == 0)
    printf("Program successfully completed.\n");
    printf("======================================\n");
  return 0;
}

int instructions() {
  printf("\n \
  Usage: ./set\n");
}

