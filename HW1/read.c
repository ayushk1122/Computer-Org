#include "read.h"
#include "matrix.h"
#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<errno.h>

int mm_read(char* filename, matrix* mat) 
{
/*
Notes:
  Use fopen to open the matrix file, then use fgets or getline to read lines
  The first line of the matrix contains integers for "#rows #columns"
  - Use these to allocate your arrays for matrix data
  Each subsequent line contains a single data value
  - You can use sscanf to parse each matrix entry
  - First value is data[0][0], next is data[0][1] ... data[#rows-1][#columns-1]
*/
  FILE* fp = fopen(filename, "r");

  //checks for valid file
  if (fp == NULL) {
    printf("mm_read: failed to open file.\n");
    exit(EXIT_FAILURE);
  }

  char* rowCol = NULL; //string to store each matrix value
  size_t len = 0;
  int check = getline(&rowCol, &len, fp);
  if (check == -1) { //checks for non empty file 
    printf("mm_read: failed to read from file.\n");
    exit(EXIT_FAILURE);
  }

  //splits first line by blank space
  char delim[] = " ";
  char *ptr = NULL;
  if ((ptr = strtok(rowCol, delim)) == NULL) { //checks to make sure row value is there
    printf("mm_read: failed to read matrix dimensions.\n");
    exit(EXIT_FAILURE);
  };

  int row = atoi(ptr); //converts first half of string to int for row value

  if ((ptr = strtok(NULL, delim)) == NULL) { //checks to make sure both row and col value are there
    printf("mm_read: failed to read matrix dimensions.\n");
    exit(EXIT_FAILURE);
  }

  int col = atoi(ptr); //converts second string to int col value
  //assigns row col values to 
  mat->rows = row;
  mat->cols = col;
  mm_alloc(mat);

  //printf("%d rows %d cols", mat->rows, mat->cols);
  int totalCount = 0;
  int rowIndex = 0;
  int colIndex = 0;
  char* eptr = NULL;

  while (getline(&rowCol, &len, fp) != -1) { //loops through rest of file and gets all matrix vals
    if (colIndex > (mat->cols - 1)) { //handles changing rows when reaches end of matrix column
      rowIndex++;
      colIndex = 0;
    }

    totalCount++; //keeps track of total input values
    mat->data[rowIndex][colIndex] = (strtod(rowCol, &eptr)); //fills matrix data
    //printf("%.1lf\n", mat->data[rowIndex][colIndex]);
    colIndex++; 
  }

  if (totalCount != (mat->rows * mat->cols)) { //checks for valid input number for matrix values
    printf("mm_read: failed to read matrix values.\n");
    exit(EXIT_FAILURE);
  }

  fclose(fp);
  free(rowCol); //frees string pointer storing values from input file

  return 0;
}