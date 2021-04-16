/*---------------------------------------------------------------------------
  This program demonstrates a simple Ax=b QR solver
  student copy 
  gcc -g  -std=c99 lab_a.c -lgsl -lgslcblas -o lab_a
---------------------------------------------------------------------------*/
#include <stdio.h>

/* GSL */
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>

#define NR (3)
#define NC (3)

#define MATRIX_INIT(m, nr, ...) do {\
    double gbl_decl_arr[][(nr)] = __VA_ARGS__; \
    for (int i = 0; i < (m)->size1; i++) {        \
        for (int j = 0; j < (m)->size2; j++){                       \
             gsl_matrix_set((m), i, j, gbl_decl_arr[i][j]);         \
        }     \
    } \
} while (0)

#define VECTOR_INIT(m, ...) do {\
    double gbl_decl_arr[] = __VA_ARGS__; \
    for (int i = 0; i < (m)->size; i++) {        \
        gsl_vector_set((m), i, gbl_decl_arr[i]); \
    } \
} while(0)


int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    gsl_matrix *A;          // The A matrix
    gsl_vector *b;          // b vector
    gsl_vector *x;          // The answer
    gsl_vector *tau;        // Householder coefficients

    // Allocate space for all the data
    A = gsl_matrix_alloc(NR, NC);
    b = gsl_vector_alloc(NC);
    x = gsl_vector_alloc(NR);
    tau = gsl_vector_alloc(NC);     // required place holder for GSL

    // Encode the following polynomial in GSL
    //2x1 + 8x2 + 6x3 = 20
    //4x1 + 2x2 - 2x3 = -2
    //3x1 -  x2 +  x3 = 11
    // encode the A matrix
    MATRIX_INIT(A, NR, {
        {2, 8, 6},
        {4, 2, -2},
        {3, -1, 1},
    });

    // Encode the b vector
    VECTOR_INIT(b, {20, -2, 11});

    /* Print matrix A and vector  b */
    printf("Matrix A (%d x %d)\n", NR, NC);
    for (int i = 0; i < NR; i++)
    {
        printf("%3d: ", i);
        for (int j = 0; j < NC; j++)
        {
            printf("%12.5f ", gsl_matrix_get(A, i, j));
        }
        printf("\n");
    }
    printf("Vector b (%d x 1)\n", NR);
    for (int i = 0; i < NR; i++)
    {
        printf("%12.5f \n", gsl_vector_get(b, i));
    }

    //  Use GSL QR decomposition to do the Ax=b math
    gsl_linalg_QR_decomp(A, tau);
    gsl_linalg_QR_solve(A, tau, b, x);

    printf("\nSolution vector x (%d x 1)\n", NR);
    for (int i = 0; i < NR; i++)
    {
        printf("%12.5f \n", gsl_vector_get(x, i));
    }
    printf("\n");

    /* Free memory - VERY IMPORTANT! */
    gsl_matrix_free(A);
    gsl_vector_free(tau);
    gsl_vector_free(b);
    gsl_vector_free(x);
}
