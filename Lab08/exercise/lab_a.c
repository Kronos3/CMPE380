/*---------------------------------------------------------------------------
  This program demonstrates a faster 2d dynamic array
  10/08/2019   R. Repka    Initial version
  student version
  
  valgrind --tool=memcheck --leak-check=yes --track-origins=yes
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include "Timers.h"

#ifndef ROWS
#define ROWS 3
#endif

#ifndef COLS
#define COLS 5
#endif

void free_all_matrix(double** A);

/* Dynamically Allocated Arrays */
int main(int argc, char* argv[])
{
    // row/col sizes
    int rows = ROWS, cols = COLS;

    // The key trick to a 2D matrix
    double** A = NULL;

    // Used for pointer  management
    double* ptr = NULL;

    DECLARE_REPEAT_VAR(AllocTimer)
    BEGIN_REPEAT_TIMING(REPEAT_NUM, AllocTimer)
    // Allocate space for array of pointers
    A = malloc(rows * sizeof(double*));

    // Allocate all matrix at ocolse
    ptr = calloc(rows * cols, sizeof(double));

    // Set array of row pointers properly
    for (int i = 0; i < rows; i++)
    {
        A[i] = ptr + cols * i;
    }

#ifndef EN_TIME
    // Initialize the matrix to nice data
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            A[i][j] = (double) i + (double) j / 10.0;
        }
    }

    // Print out the matrix
    printf("The %dx%d 2D dynamic matrix\n", rows, cols);
    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            printf("%4.1f\t", A[i][j]);
        }
        printf("\n");
    }
#endif

    // Complex return of space
    free_all_matrix(A);

    END_REPEAT_TIMING
    PRINT_RTIMER(AllocTimer, REPEAT_NUM)

    return 0;
}

/*---------------------------------------------------------------------------
  This function frees a 2D matrix generated using block row allocations
  
  where:  double **M - Point to an array of pointers to free  
  Returns: nothing
  Error handling: none
---------------------------------------------------------------------------*/
void free_all_matrix(double** A)
{
    free(A[0]);  /* free array of elements     */
    A[0] = NULL;
    free(A);       /* free array of ptrs to rows */
    A = NULL;
}
