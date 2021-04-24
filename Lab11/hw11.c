/********************************************************************
 * Applied Programming:                                             
 *    Solution of Overdetermined System of Equations Ax=b arising   
 *    in least square problems via QR factorizations using the GSL
 *
 * Compilation:  gcc -ansi -g -lgsl -lgslcblas  hw8.c DynamicArrays.c -o  hw8
 *                                                                  
 * Tested in Ubuntu 12.04 LTS                                       
 * Revised: Juan C. Cockburn, April 9, 2015 (juan.cockburn@rit.edu) 
 * 10/10/2015 R. Repka - Minor clean up
 * 10/26/2016 R. Repka - Major re-write, added QR and norm solution modes
 * 11/12/2017 R. Repka - Minor comment change for norm of residuals, removed
 *                       "c" in GE_FindPoint
 * 11/28/2016 R. Repka - Added Pearson function
 * 07/11/2017 R. Repka - Switched to getopt_long_only
 * 04/20/2018 R. Repka - removed QR solution, added better problem
 *                       description
 * 12/11/2018 R. Repka - Minor comment changes
 * 04/10/2019 R. Repka - Don't need evalPoly() anymore
 ********************************************************************/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <gsl/gsl_blas.h>
#include <gsl/gsl_vector.h>
#include <gsl/gsl_matrix.h>
#include <gsl/gsl_linalg.h>
#include "ClassErrors.h"
#include "DynamicArrays.h"

#define MATRIX_INIT(m, nr, ...) do {\
    double gbl_decl_arr[][(nr)] = __VA_ARGS__; \
    for (int i = 0; i < (m)->size1; i++) {        \
        for (int j = 0; j < (m)->size2; j++){                       \
             gsl_matrix_set((m), i, j, gbl_decl_arr[i][j]);         \
        }     \
    } \
} while (0)

/*****************************************************************************
 Function prototypes 
*****************************************************************************/
void readPoints(FILE *file, DArray *DArrayPtr);

void Norm_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls,
                    int verbose);


void vector_print(const gsl_vector *self, const char *name)
{
    if (name)
    {
        printf("%s (%zu x 1)\n", name, self->size);
    }

    for (int i = 0; i < self->size; i++)
    {
        printf("%3d: %12.5f\n", i, gsl_vector_get(self, i));
    }
}

void matrix_print(const gsl_matrix *self, const char *name)
{
    if (name)
    {
        printf("%s (%zu x %zu)\n", name, self->size1, self->size2);
    }

    for (int i = 0; i < self->size1; i++)
    {
        printf("%3d: ", i);
        for (int j = 0; j < self->size2; j++)
        {
            printf("%12.5f ", gsl_matrix_get(self, i, j));
        }
        printf("\n");
    }
}


/*****************************************************************************
 This program uses least squares to generate approximate functions
    usage: hw12   -order  num   -points  file  [-verbose] \n");
 
 Returns: 0 for success, non-zero for errors
 *****************************************************************************/
int main(int argc, char *argv[])
{
    DArray points;
    FILE *pointsFile;
    int i;
    int nr, nc;       /* number of rows and cols  */
    gsl_vector *x_ls; /* least squares solution   */

    int verbose = 0, help = 0;
    int order = 0;
    const char *points_fn = NULL;

    /*------------------------------------------------------------------------
      add your UI variables with sentential values and add your get_opt_long
      code here
    ------------------------------------------------------------------------*/
    char *getoptOptions = "vhp:o:";    /* add lots of stuff here */

    /* This contains the long command line parameter list, it should mostly
      match the short list                                                  */
    struct option long_options[] = {
            {"order",   required_argument, 0, 'o'},
            {"points",  required_argument, 0, 'p'},
            {"verb",    no_argument,       0, 'v'},
            {"verbose", no_argument,       0, 'v'},

            // Help
            {"help",    no_argument,       0, 'h'},
            {0, 0,                         0, 0}
    };

    int rc;
    int option_index;
    opterr = 1;           /* Enable automatic error reporting */
    while ((rc = getopt_long_only(argc, argv,
                                  getoptOptions, long_options,
                                  &option_index)) != -1)
    {
        //     printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, option_index);
        /* Detect the end of the options. */
        switch (rc)
        {
            case 'o':
                order = (int) strtol(optarg, NULL, 0);
                if (order <= 0)
                {
                    fprintf(stderr, "Invalid polynomial order '%d', must be "
                                    "greater than or equal to 1\n\n", order);
                }
                break;
            case 'p':
                points_fn = optarg;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'h':
                help = 1;
                break;
            case '?':  /* Handled by the default error handler */
                break;
            default:
                printf("Internal error: undefined option %0xX\n", rc);
                return PGM_INTERNAL_ERROR;
        }
    }


    /*------------------------------------------------------------------------
      Check for command line syntax errors
    ------------------------------------------------------------------------*/
    if (help || !points_fn || order < 1)
    {
        fprintf(stderr,
                "This program uses least squares to generate approximate functions for data.\n"
                "usage: hw11 -o[rder] num -p[oints] file [-v[erb[ose]]]\n"
                "\n"
                "Options:\n"
                "    -p, -points  file        [required] file with data points (2 col data)\n"
                "    -o, -order   number      [required] order of equation to use. >= 1\n"
                "    -v, -verbose             Print intermediary results\n"
                "    -h, -help                Print help and exit\n"
                "\n"
                "  e.g: ./hw12 -order 2 -points points.txt\n"
                "  use Norm Gaussian elimination for a 2nd order equation\n");

        // Don't exit with error on help
        return !help;
    } /* End if error */

    /* The real matrix size is one larger than the order */
    nc = order + 1;

    /* Create a dynamic array to hold the data points */
    CreateDArray(&points, GROWTH_AMOUNT);

    /* Try to open the data points file */
    pointsFile = fopen(points_fn, "r");
    if (NULL == pointsFile)
    {
        fprintf(stderr, "Error: unable to open data points file '%s'\n",
                points_fn);
        fflush(stderr);
        DestroyDArray(&points);
        return (PGM_FILE_NOT_FOUND);
    }

    /* Read the data in */
    readPoints(pointsFile, &points);
    fclose(pointsFile);
    nr = (int) points.EntriesUsed;

    /* Contains the final solution */
    x_ls = gsl_vector_alloc(nc);

    /* Execute the right algorithm */
    Norm_FindPoint(nr, nc, &points, x_ls, verbose);
    printf("Least Squares Solution via Norm factorization:\n");

    /* Print out the variable coefficients */
    if (verbose)
    {
        for (i = 0; i < nc; i++)
        {
            printf(" x_ls[%1d] = %20.16f \n", i, gsl_vector_get(x_ls, i));
        }
        printf("\n");
    }

    /* Print out the pretty answers */
    printf("  f(x) = ");
    for (i = 0; i < nc; i++)
    {
        if (0 == i)
        {
            printf(" %g +", gsl_vector_get(x_ls, i));
        }
        else if (1 == i)
        {
            printf(" %gx ", gsl_vector_get(x_ls, i));
            if (i < (nc - 1))
            {
                printf("+");
            }
        }
        else
        {
            printf(" %gx^%d ", gsl_vector_get(x_ls, i), i);
            if (i < (nc - 1))
            {
                printf("+");
            }
        }
    } /* End for i */
    printf("\n\n");

    /* Cleanup */
    gsl_vector_free(x_ls);
    DestroyDArray(&points);

    return PGM_SUCCESS; /* main */
}

/*---------------------------------------------------------------------------
  Find the least squares approximation to data "points" of order "nc" using
  the "Normal equations" approach.
        
                        A'Az = A'b
  
  Where: int nr           - The number of points (rows) of the input file
         int nc           - The number of columns (order) of the solution
         DArray *points   - Pointer to the x,y data
         gsl_vector *x_ls - The solution is returned here
         int verbose      - Verbose flag

  Returns: nothing
  Errors: Assumes the standard GSL error handler
---------------------------------------------------------------------------*/
void Norm_FindPoint(int nr, int nc, const DArray *points, gsl_vector *x_ls,
                    int verbose)
{
    int i, j;         /* counters                 */
    gsl_matrix *A;    /* coefficient matrix A     */
    gsl_matrix *AT;   /* coefficient matrix A'    */
    gsl_matrix *ATA;  /* coefficient matrix A'A   */
    gsl_vector *b;    /* coefficient vector b     */
    gsl_vector *ATB;  /* coefficient vector A'b   */
    gsl_vector *tau;  /* Householder coefficients */
    gsl_vector *res;  /* residual vector */

    /* Allocate space for Matrices and vectors */
    ATA = gsl_matrix_alloc(nc, nc); /* Data matrix */
    AT = gsl_matrix_alloc(nc, nr);  /* Data matrix */
    A = gsl_matrix_alloc(nr, nc);   /* Data matrix */
    b = gsl_vector_alloc(nr);       /* Data vector */
    ATB = gsl_vector_alloc(nc);     /* Data vector but rotated so nc not nr */
    tau = gsl_vector_alloc(nc);     /* required place holder for GSL, nc */
    res = gsl_vector_alloc(nc);

    /* Initialize Matrix A with the data in "points"  */
    /* Remember to make the first column of data 1's */
    for (i = 0; i < nr; i++)
    {
        double accum_prod = 1;
        for (j = 0; j < nc; j++)
        {
            gsl_matrix_set(A, i, j, accum_prod);
            accum_prod *= points->Payload[i].x;
        }
    }

    /* Initialize vector b with y data */
    for (i = 0; i < nr; i++)
    {
        gsl_vector_set(b, i, points->Payload[i].y);
    }

    // Calculate intermediate matrices
    gsl_matrix_transpose_memcpy(AT, A);
    gsl_blas_dgemm(CblasNoTrans, CblasNoTrans, 1.0, AT, A, 0.0, ATA);
    gsl_blas_dgemv(CblasNoTrans, 1.0, AT, b, 0.0, ATB);

    if (verbose)
    {
        matrix_print(A, "A");
        vector_print(b, "B");
        matrix_print(AT, "AT");
        matrix_print(ATA, "ATA");
        vector_print(ATB, "ATB");
    }

    // Solve using QR factorization
    gsl_linalg_QR_decomp(ATA, tau);
    gsl_linalg_QR_lssolve(ATA, tau, ATB, x_ls, res);

    /* Free memory  */
    gsl_matrix_free(ATA);
    gsl_matrix_free(AT);
    gsl_matrix_free(A);
    gsl_vector_free(b);
    gsl_vector_free(ATB);
    gsl_vector_free(tau);
    gsl_vector_free(res);
} /* end Norm_FindPoint() */



/***************************************************************************************
 Reads the points data from file and returns it in a Darray
                       
 
 Where: FILE *file     - open handle to read from
                         of the form:     22.0      6.7
                                          23.4      18.8
        DArray *DArrayPtr - Pointer to a dynamic array to store the data
  Returns: nothing
  Errors:  none
*****************************************************************************************/
void readPoints(FILE *file, DArray *DArrayPtr)
{
    char String[256];
    Data tempData;

    /* Grab the data and put it in a Dynamic Array */
    while (fgets(String, 256, file))
    {
        char *tok = strtok(String, " ");
        tempData.x = strtod(tok, NULL);
        tok = strtok(NULL, " ");
        tempData.y = strtod(tok, NULL);
        PushToDArray(DArrayPtr, &tempData);
    }
} /* readPoints */
