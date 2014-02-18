#include <stdio.h>
#include <stdlib.h>

#define ARGS 3
#define DATAPOINTS 76800
#define ROWS 240
#define COLS 320

int instructions();
int exit_program(int);
void print_array(int * data);
int convolution(int a,int b,int c,int d,int e,int f,int g,int h,int i, int poi,int *data);

// take arguments for sort number and data file
int main(int argc, char *argv[]) {
 
  if (argc != ARGS)
    return instructions();

  int i;
  int convnum = atoi(argv[1]);
  FILE * filein = fopen(argv[2],"r");
  if (filein == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(1);
    return 0;
  }

  printf("======================================\n");
  printf("\tCONVOLUTION TEST PROGRAM\n");
  printf("======================================\n\n");

  int row,col;
  int in_array[DATAPOINTS];
  int out_array[DATAPOINTS];
  for (i = 0; i < DATAPOINTS; i++) {
    fscanf(filein, "%d ", &in_array[i]);
  }

  switch(convnum) {
    case 1:
      printf("#1 - Blur\n");
      i = 0;
      for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
          out_array[i] = convolution(1,1,1,1,1,1,1,1,1,i,in_array);
          i++;
        }
      }
      break;
    case 2:
      printf("#2 - Edge\n");
      i = 0;
      for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
          out_array[i] = convolution(0,1,0,1,-4,1,0,1,0,i,in_array);
          i++;
        }
      }
      break;
    case 3:
      printf("#3 - Emboss\n");
      i = 0;
      for (row = 0; row < ROWS; row++) {
        for (col = 0; col < COLS; col++) {
          out_array[i] = convolution(-2,-1,0,-1,1,1,0,1,2,i,in_array);
          i++;
        }
      }
      break;
    default:
      printf("Improper sort number. Range 1-3.\n");
  }

  FILE * fileout = fopen("output.txt","w");
  if (fileout == NULL) {
    printf("File did not open properly! Does it exist?\n");
    exit_program(1);
    return 0;
  }
  
  for (i = 0; i < DATAPOINTS; i++) {
    fprintf(fileout, "%d ", out_array[i]);
  }

  exit_program(0);
}

int convolution(int a,int b,int c,int d,int e,int f,int g,int h,int i,int poi,int *data) {
  int t = 0;

  if (poi < 322 || poi > 76478) 
    t = poi;
  else {
    t = a*data[poi-ROWS-1] + b*data[poi-ROWS] + c*data[poi-ROWS+1] 
      + d*data[poi-1] + e*data[poi] + f*data[poi+1]
      + g*data[poi+ROWS-1] + h*data[poi+ROWS] + i*data[poi+ROWS+1];
    t = t/9;
  }
  return t;
}

void print_array(int * data) {
  int i;
  for (i = 0; i < DATAPOINTS; i++) {
    printf("%d ",data[i]);
  }
  printf("\n\n");
}

int exit_program(int result) {
  if (result == 1)
    printf("Program was not successful.\n\n");
  if (result == 0)
    printf("Program successfully completed.\n\n");
    printf("======================================\n");
  return 0;
}

int instructions() {
  printf("\n \
  Usage: ./main <convolution> <data file>\n \
  Choose a convolution operation from the following list:\n \
  1 - Blur\n \
  2 - Edge \n \
  3 - Emboss \n");
}

