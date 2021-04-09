/**************************************************************************
  quadTest.c - for the student
  This program demonstrates C99's complex support.
 
 12/22/2018 - R. Repka  Initial version
 03/27/2019 - R. Repka  b**2 typo
**************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <math.h>
#include </usr/include/complex.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"


/************************************************************************
   Simple code to test complex support  
************************************************************************/
int main(int argc, char *argv[])
{
    int coeff_set[3] = {0};
    double coeff[3] = {0};
    int help = 0;

    /* This contains the short command line parameters list */
    char *getoptOptions = "a:b:c:h";    /* add lots of stuff here */

    /* This contains the long command line parameter list, it should mostly
      match the short list                                                  */
    struct option long_options[] = {
            {"numa",      required_argument, 0, 'a'},
            {"numb",      required_argument, 0, 'b'},
            {"numc",      required_argument, 0, 'c'},

            // Help
            {"help",      required_argument, 0, 'h'},
            {0, 0,                           0, 0}
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
            case 'a':
            case 'b':
            case 'c':
                coeff[rc - 'a'] = strtod(optarg, NULL);
                coeff_set[rc - 'a'] = 1;
                break;
            case 'h':
                help = 1;
                break;

            case '?':  /* Handled by the default error handler */
                break;
            default:
                printf("Internal error: undefined option %0xX\n", rc);
                return PGM_INTERNAL_ERROR;
        } // End switch
    } /* end while */

    if (!help)
    {
        if (!(coeff_set[0] && coeff_set[1] && coeff_set[2]))
        {
            printf("All coefficients must be provided\n");
            help = 1;
        }

        if (coeff[0] == 0.0)
        {
            printf("Coefficient 'A' must not be zero\n");
            help = 1;
        }
    }

    if (help)
    {
        fprintf(stderr,
                "Performs complex root finding for quadratic equations\n"
                "usage: hw7 -a[num] REAL -b[num] REAL -c[num] REAL\n"
                "\n"
                "Coefficients (all required):\n"
                "  -a -anum REAL        Provide the coefficient for x^2\n"
                "  -b -bnum REAL        Provide the coefficient for x\n"
                "  -c -cnum REAL        Provide the constant\n"
                "\n"
                "  -h --help            Print this help and exit\n"
        );
        return PGM_INTERNAL_ERROR;
    }

    polynomial p;
    initPoly(&p, 3);
    p.polyCoef[0] = coeff[0];
    p.polyCoef[1] = coeff[1];
    p.polyCoef[2] = coeff[2];
    double complex roots[2];

    DECLARE_REPEAT_VAR(Timer)
    BEGIN_REPEAT_TIMING(TIM_ITER, Timer)

    quadraticRoots(&p, roots);

    END_REPEAT_TIMING
    PRINT_RTIMER(Timer, TIM_ITER)

    printf("Roots of %fx**2 + %fx + %f\n"
           "    root_1: %f + %fi\n"
           "    root_2: %f + %fi\n",
           coeff[0], coeff[1], coeff[2],
           creal(roots[0]), cimag(roots[0]),
           creal(roots[1]), cimag(roots[1]));

    freePoly(&p);
    return PGM_SUCCESS;
} /* End main */


/*---------------------------------------------------------------------------
 Returns the two roots from a quadratic polynomial

   =  -b +-sqrt(b**2-4ac)
      -------------------
               2a

 Where: polynomial *p              - Pointer to a 2nd order polynomial
 Returns: double complex* rootList - A pointer to a complex root pair.
 Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void quadraticRoots(polynomial *p, double complex rootList[2])
{
    double complex disc = (p->polyCoef[1] * p->polyCoef[1]) -
            4 * (p->polyCoef[0]) * (p->polyCoef[2]);
    double complex sq = csqrt(disc);

    rootList[0] = (-p->polyCoef[1] + sq) / (2 * p->polyCoef[0]);
    rootList[1] = (-p->polyCoef[1] - sq) / (2 * p->polyCoef[0]);
}

/*---------------------------------------------------------------------------
  This function allocates an array of "nterm" double complex elements and then
  initializes the terms of the polynomial to NAN (not a number).
  We initialize the terms of the polynomial to NAN to prevent the users from
  accidentally using the polynomial before valid values are set.
  
  Where: polynomial *p       - Pointer to polynomial data structure to create
         unsigned int nterms - The number of elements to create
  Returns: nothing
  Errors:  prints an error and exits
---------------------------------------------------------------------------*/
void initPoly(volatile polynomial *p, unsigned int nterms)
{
    p->nterms = nterms;
    p->polyCoef = malloc(sizeof(double complex) * nterms);
    for (int i = 0; i < nterms; i++)
    {
        p->polyCoef[i] = NAN;
    }
}

/*---------------------------------------------------------------------------
  Destroys/frees a polynomial
  
  Where: polynomial *p - Pointer to polynomial data structure to destroy
  Returns: nothing
  Errors:  none
---------------------------------------------------------------------------*/
void freePoly(polynomial *p)
{
    free(p->polyCoef);
}
