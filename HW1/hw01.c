/* 
Assignment 1: NxM Matrix Multiplication 
CSCI-2500 Fall 2021
Ayush Krishnappa 
RIN: 662028478
*/

/******************************************************************************/
/* Usual suspects to include  */
/******************************************************************************/
#include<stdio.h>
#include<stdlib.h>
#include<unistd.h>
#include<string.h>
#include<errno.h>
#include "matrix.h"
#include "mult.h"
#include "print.h"
#include "alloc.h"
#include "read.h"
/******************************************************************************/
/* Function Calls */
/******************************************************************************/

// typedef struct{
//   double** data;
//   int rows;
//   int cols;
// } matrix; 

//int mm_alloc(matrix* mat);
int mm_free (matrix* mat);
//int mm_print(matrix* mat);
//int mm_read (char* filename, matrix* mat);
//matrix* mm_matrix_mult(matrix* mat1, matrix* mat2);

// int mm_alloc(matrix* mat)
// {
// /*
// Notes:
//   Allocate an array of pointers to a double
//   Then, allocate an row/array of doubles and assign each double pointer that row
//   Make sure you're allocating using mat->data in the matrix struct
//   You can use calloc for allocation and zeroing out all the bytes
//   - otherwise, you'd want to manually set all arrays to 0 (good practice)
//   If your malloc/calloc returns null, the allocation failed
// */
//   if (mat->rows < 0 || mat->cols < 0) { //checks for valid rows and cols
//     printf("mm_alloc: allocation failed.\n");
//     exit(EXIT_FAILURE);
//   }

//   //allocates memory for matrix
//   double** columns = (double**)calloc(mat->rows, sizeof(double));
//   for (int i = 0; i < mat->rows; i++) {
//     double* row = (double*)calloc(mat->cols, sizeof(double));
//     columns[i] = row;
//   }

//   mat->data = columns; 
  
//   return 0;
// }

int mm_free(matrix* mat)
{
/*
Notes:
  Reverse the allocation process, but you'll be calling free() instead
*/
  for (int i = 0; i < mat->rows; i++) { //frees matrix allocated memory 
    free(mat->data[i]);
  }

  free(mat->data);
  
  return 0;
}

// int mm_print(matrix* mat)
// {
// /*
// Notes:
//   Print all the data values in your matrix
//   Go row by row
//   At the end of each row, print a newline
//   Between each data value, insert a tab
//   Use 2 digits of precision after the decimal (use "%10.2lf\t" as format string)
//   See output.txt for expected output formatting
// */

//   //prints contents of matrix in formatted form 
//   printf("\n/******** START of 2-D Matrix **********************************/\n");
//   for (int i = 0; i < mat->rows; i++) {
//     for (int j = 0; j < mat->cols; j++) {
//       printf("%10.2lf\t", mat->data[i][j]);
//     }
//     printf("\n");
//   }
//   printf("/******** END of 2-D Matrix ************************************/\n");

//   return 0;
// }

// int mm_read(char* filename, matrix* mat) 
// {
// /*
// Notes:
//   Use fopen to open the matrix file, then use fgets or getline to read lines
//   The first line of the matrix contains integers for "#rows #columns"
//   - Use these to allocate your arrays for matrix data
//   Each subsequent line contains a single data value
//   - You can use sscanf to parse each matrix entry
//   - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
// */
//   FILE* fp = fopen(filename, "r");

//   //checks for valid file
//   if (fp == NULL) {
//     printf("mm_read: failed to open file.\n");
//     exit(EXIT_FAILURE);
//   }

//   char* rowCol = NULL; //string to store each matrix value
//   size_t len = 0;
//   int check = getline(&rowCol, &len, fp);
//   if (check == -1) { //checks for non empty file 
//     printf("mm_read: failed to read from file.\n");
//     exit(EXIT_FAILURE);
//   }

//   //splits first line by blank space
//   char delim[] = " ";
//   char *ptr = NULL;
//   if ((ptr = strtok(rowCol, delim)) == NULL) { //checks to make sure row value is there
//     printf("mm_read: failed to read matrix dimensions.\n");
//     exit(EXIT_FAILURE);
//   };

//   int row = atoi(ptr); //converts first half of string to int for row value

//   if ((ptr = strtok(NULL, delim)) == NULL) { //checks to make sure both row and col value are there
//     printf("mm_read: failed to read matrix dimensions.\n");
//     exit(EXIT_FAILURE);
//   }

//   int col = atoi(ptr); //converts second string to int col value
//   //assigns row col values to 
//   mat->rows = row;
//   mat->cols = col;
//   mm_alloc(mat);

//   //printf("%d rows %d cols", mat->rows, mat->cols);
//   int totalCount = 0;
//   int rowIndex = 0;
//   int colIndex = 0;
//   char* eptr = NULL;

//   while (getline(&rowCol, &len, fp) != -1) { //loops through rest of file and gets all matrix vals
//     if (colIndex > (mat->cols - 1)) { //handles changing rows when reaches end of matrix column
//       rowIndex++;
//       colIndex = 0;
//     }

//     totalCount++; //keeps track of total input values
//     mat->data[rowIndex][colIndex] = (strtod(rowCol, &eptr)); //fills matrix data
//     //printf("%.1lf\n", mat->data[rowIndex][colIndex]);
//     colIndex++; 
//   }

//   if (totalCount != (mat->rows * mat->cols)) { //checks for valid input number for matrix values
//     printf("mm_read: failed to read matrix values.\n");
//     exit(EXIT_FAILURE);
//   }

//   fclose(fp);
//   free(rowCol); //frees string pointer storing values from input file

//   return 0;
// }

// matrix* mm_matrix_mult(matrix* mat1, matrix* mat2)
// {
// /*
// Notes:
//   First check that it is actually possible to multiply the two inputs
//   If so, construct a result matrix
//   Then perform the multiplication
//   Then return a pointer to your result matrix
// */

//   if (mat1->cols != mat2->rows) { //checks for invalid matrix dimensions
//     printf("mm_matrix_mult: dimension mismatch between matrices.\n");
//     exit(EXIT_FAILURE);
//   }

//   matrix* result_matrix = malloc(sizeof(matrix)); //result matrix allocates memory
//   result_matrix->rows = mat1->rows;
//   result_matrix->cols = mat2->cols;

  mm_alloc(result_matrix); //allocates memory for row and col size of result matrix
  for (int i = 0; i < result_matrix->rows; i++) { //loops through rows
    for (int j = 0; j < result_matrix->cols; j++) { //loops through cols
      for (int k = 0; k < mat1->cols; k++) { //loops matrix 1 column times
        result_matrix->data[i][j] += (mat1->data[i][k] * mat2->data[k][j]); //calcs dot products 
      }
    }
  }

//   return result_matrix;
// }

int main()
{
  /*
   * you must keep this function AS IS. 
   * you cannot modify it!
   */
  char filename_A[256];
  char filename_B[256];
  matrix* A = (matrix*)malloc(sizeof(matrix));
  matrix* B = (matrix*)malloc(sizeof(matrix));
  matrix* C = NULL;

  printf("Please enter the matrix A data file name: ");
  if (1 != scanf("%s", filename_A)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_A, A);
  mm_print(A);
  
  printf("Please enter the matrix B data file name: ");
  if (1 != scanf("%s", filename_B)) {
    printf("main: scanf failed to read filename\n");
    exit(-1);
  }
  mm_read(filename_B, B);
  mm_print(B);
  
  C = mm_matrix_mult(A, B);
  mm_print(C);

  mm_free(A);
  mm_free(B);
  mm_free(C);
  free(A);
  free(B);
  free(C);
  
  return 0;
}
