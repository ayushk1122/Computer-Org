#include "alloc.h"
#include "matrix.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int mm_alloc(matrix* mat)
{
/*
Notes:
  Allocate an array of pointers to a double
  Then, allocate an row/array of doubles and assign each double pointer that row
  Make sure you're allocating using mat->data in the matrix struct
  You can use calloc for allocation and zeroing out all the bytes
  - otherwise, you'd want to manually set all arrays to 0 (good practice)
  If your malloc/calloc returns null, the allocation failed
*/
  if (mat->rows < 0 || mat->cols < 0) { //checks for valid rows and cols
    printf("mm_alloc: allocation failed.\n");
    exit(EXIT_FAILURE);
  }

  //allocates memory for matrix
  double** columns = (double**)calloc(mat->rows, sizeof(double));
  for (int i = 0; i < mat->rows; i++) {
    double* row = (double*)calloc(mat->cols, sizeof(double));
    columns[i] = row;
  }

  mat->data = columns; 
  
  return 0;
}