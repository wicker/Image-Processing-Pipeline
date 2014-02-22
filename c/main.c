#include <stdio.h>
#include <stdlib.h>

#define ARGS 4
#define DATAPOINTS 76800
#define ROWS 320 
#define COLS 240

// TO DO
// 1. Extensible convolution that receives array of integer coefficients
// and indicated size of convolution matrix, then checks to make sure
// the two values agree. If they do, it performs the convolution on 
// the given image. 
//
// 2. Let the program run more than one convolution operation in a row.

int instructions();
int exit_program(int);
void print_array(int * data);
int blur(int sz,int *indata,int *outdata);
int conv_3x3(int a,int b,int c,int d,int e,int f,int g,int h,int i, int poi,int row, int col, int *data);
int conv_5x5(int a,int b,int c,int d,int e,
             int f,int g,int h,int i,int j,
             int k,int l,int m,int n,int o,
             int p,int q,int r,int s,int t,
             int u,int v,int w,int x,int y,
             int poi,int row, int col, int *data);

// take arguments for sort number and data file
int main(int argc, char *argv[]) {
 
  if (argc != ARGS)
    return instructions();

  int i;
  int sz = atoi(argv[3]);
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
      i = 1;
      for (row = 1; row <= ROWS; row++) {
        for (col = 1; col <= COLS; col++) {
          out_array[i] = conv_5x5(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,i,row,col,in_array);
          i++;
        }
      }
      break;
    case 2:
      printf("#2 - Edge\n");
      i = 1;
      for (row = 1; row <= ROWS; row++) {
        for (col = 1; col <= COLS; col++) {
          out_array[i] = conv_3x3(0,1,0,1,-4,1,0,1,0,i,row,col,in_array);
          i++;
        }
      }
      break;
    case 3:
      printf("#3 - Emboss\n");
      i = 1;
      for (row = 1; row <= ROWS; row++) {
        for (col = 1; col <= COLS; col++) {
          out_array[i] = conv_3x3(-2,-1,0,-1,1,1,0,1,2,i,row,col,in_array);
          i++;
        }
      }
      break;
    case 4:
      printf("#4 - Blur Function Test\n");
      int rtn = blur(sz,in_array,out_array);
      if (rtn == -1) 
        printf("Fail!");
      else break;
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

int blur(int sz,int *indata,int *outdata) {

  int i,row,col;

  i = 1;
  for (row = 1; row <= ROWS; row++) {
    for (col = 1; col <= COLS; col++) {
      if (sz == 3)
        outdata[i] = conv_5x5(1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,i,row,col,indata);
      else if (sz == 5)
        outdata[i] = conv_3x3(1,1,1,1,1,1,1,1,1,i,row,col,indata);
      else
        outdata[i] = indata[i];
      i++;
    }
  }

  return 0;
}

int conv_3x3(int a,int b,int c,int d,int e,int f,int g,int h,int i,int poi,int row, int col, int *data) {
  int t = 0;
  if (row == 1 || row == ROWS || col == 1 || col == COLS) 
    t = data[poi];
  else {
    t = a*data[poi-COLS-1] + b*data[poi-COLS] + c*data[poi-COLS+1] 
      + d*data[poi-1] + e*data[poi] + f*data[poi+1]
      + g*data[poi+COLS-1] + h*data[poi+COLS] + i*data[poi+COLS+1];
    t = t/9;
  }
  return t;
}

int conv_5x5(int a,int b,int c,int d,int e,
             int f,int g,int h,int i,int j,
             int k,int l,int m,int n,int o,
             int p,int q,int r,int s,int t,
             int u,int v,int w,int x,int y,
             int poi,int row, int col, int *data) {
  int out = 0;
  if (row <= 2 || row >= ROWS-1 || col <= 1 || col >= COLS-1) 
    out = data[poi];
  else {
    out = a*data[poi-2*COLS-2] + b*data[poi-2*COLS-1] + c*data[poi-2*COLS] + d*data[poi-2*COLS+1] + e*data[poi-2*COLS+2]
      + f*data[poi-COLS-2] + g*data[poi-COLS-1] + h*data[poi-COLS] + i*data[poi-COLS+1] + j*data[poi-COLS+2]
      + k*data[poi-2] + l*data[poi-1] + m*data[poi] + n*data[poi+1] + o*data[poi+2]
      + p*data[poi+COLS-2] + q*data[poi+COLS-1] + r*data[poi+COLS] + s*data[poi+COLS+1] + t*data[poi+COLS+2]
      + u*data[poi+2*COLS-2] + v*data[poi+2*COLS-1] + w*data[poi+2*COLS] + x*data[poi+2*COLS+1] + y*data[poi+2*COLS+2];
    out = out/25;
  }
  return out;
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

