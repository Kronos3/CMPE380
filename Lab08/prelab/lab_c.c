/*---------------------------------------------------------------------------
  This program demonstrates a simple 2d dynamic array
  10/08/2019   R. Repka    Initial version
  student version
  
  valgrind --tool=memcheck --leak-check=yes --track-origins=yes
---------------------------------------------------------------------------*/
#include<stdio.h>
#include <stdlib.h>

void free_matrix_space(double **M, int rows);
void printMat(double **M, int rows, int cols);


/* Dynamically Allocated Arrays */
int main(int argc, char *argv[]) {
   // row/index counters and row/col sizes
   int i,j;
   int rows=3,cols=5;

   // The key trick to a 2D matrix
   double **A = NULL;

   // Allocate space for array of pointers
   A = malloc( rows * sizeof(double *));

   // Allocate space for matrix dat elements
   for (i=0; i< rows; i++){
      A[i] = malloc(cols * sizeof(double) );
   }

   // Initialize the matrix to nice data
   for (i = 0; i < rows; i++) {
      for (j = 0; j < cols; j++) {
         A[i][j] = (double)i +(double)j/10.0;
      }
   }

   // Print out the 3x5 matrix
   printMat(A, rows, cols);


#if 0
   // Add 5 rows to the existing matrix when instructed
   // You must grow the matrix NOT create a new one
   // Initialize the NEW data as before but with a negative value
   // to show that you preserved the old data.
   // Print out the new 8x5 matrix when instructed
   int old_rows = rows;
   rows += 5;
   A = realloc(A, sizeof(double*) * rows);
   for (i = old_rows; i < rows; i++)
   {
       A[i] = malloc(sizeof(double) * cols);
   }

    for (i = old_rows; i < rows; i++) {
        for (j = 0; j < cols; j++) {
            A[i][j] = -i +(double)j/10.0;
        }
    }
#endif

    printMat(A, rows, cols);

   // The simple return of space
   // free(A);

   // Complex return of space
   free_matrix_space(A, rows);

  return 0;
}

/*---------------------------------------------------------------------------
  This function frees a 2D matrix generated using individual row allocations
  
  where:  double **M - Point to an array of pointers to free  
          int row    - The number of array row elments in M
  Returns: nothing
  Error handling: none
---------------------------------------------------------------------------*/
void free_matrix_space(double **M, int rows) {
    for (int i = 0; i < rows; i++)
    {
        free(M[i]);
        M[i] = NULL;
    }
    free(M);
    M = NULL;
}

/*---------------------------------------------------------------------------
  This function prints the contents of a 2d array
  
  where:  double **M - Point to the matrix row pointer holder  
          int row    - The number of array row elements
		  int cols	 - The number of array column elements
  Returns: nothing
  Error handling: none
---------------------------------------------------------------------------*/
void  printMat(double **M, int rows, int cols) {
	int i,j;
   // Print out the matrix
   printf("The %dx%d 2D dynamic matrix\n", rows, cols);
   for (i=0; i< rows; i++){
      for (j=0; j< cols; j++){
         printf("%4.1f\t", M[i][j]);
      }
      printf("\n");
   }
}
