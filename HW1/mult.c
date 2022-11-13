#include "mult.h"
#include "matrix.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

matrix* mm_matrix_mult(matrix* mat1, matrix* mat2)
{
/*
Notes:
  First check that it is actually possible to multiply the two inputs
  If so, construct a result matrix
  Then perform the multiplication
  Then return a pointer to your result matrix
*/

  if (mat1->cols != mat2->rows) { //checks for invalid matrix dimensions
    printf("mm_matrix_mult: dimension mismatch between matrices.\n");
    exit(EXIT_FAILURE);
  }

  matrix* result_matrix = malloc(sizeof(matrix)); //result matrix allocates memory
  result_matrix->rows = mat1->rows;
  result_matrix->cols = mat2->cols;

  mm_alloc(result_matrix); //allocates memory for row and col size of result matrix
  for (int i = 0; i < result_matrix->rows; i++) { //loops through rows
    for (int j = 0; j < result_matrix->cols; j++) { //loops through cols
      for (int k = 0; k < mat1->cols; k++) { //loops matrix 1 column times
        result_matrix->data[i][j] += (mat1->data[i][k] * mat2->data[k][j]); //calcs dot products 
      }
    }
  }

  return result_matrix;
}