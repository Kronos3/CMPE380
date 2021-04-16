/**************************************************************************
  intTest.c - for students
  This program demonstrates the speed advantages of integer versus floating
  point math.
 12/28/2015 - R. Repka  Initial version
 10/12/2016 - R. Repka  Switched to PRINT_RTIMER() macro
 03/25/2018 - R. Repka  Switched to timing macros 
 11/18/2019 - R. Repka  Polynomial comment error 
**************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"

/****************************************************************************
  Use this to define the number of iterations in your performance measurements
  ----------- You will need to adjust this number  --------
****************************************************************************/
#ifndef ITERATIONS
#define ITERATIONS   (500)
#endif

/* Optional verbose flag, compile with -DVERBOSE or not */
#ifdef VERBOSE
#undef VERBOSE
#define VERBOSE (1)
#else
#define VERBOSE (0)
#endif

/****************************************************************************
  Remember to set the QN value in rootfinding.h
****************************************************************************/

/************************************************************************
   Simple code to test Qn performance vs floating point  
************************************************************************/
int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    double Result;          /* The result from floating point calculation   */
    long iResult;          /* The result from floating point calculation   */

    /* The bisection guesses and tolerance value                        */
    double a = -25.0;
    double b = 25.0;
    double tol = 0.000001;

    /* You many need to modify the timing code for your macro implementation! */
    DECLARE_REPEAT_VAR(Qn_timer);

    /************************************************************************
     Run the Qn format test 
    ************************************************************************/
    /* You many need to modify the timing code for your macro implementation! */
    BEGIN_REPEAT_TIMING(ITERATIONS, Qn_timer)
    iResult = ibisection(iPoly,
                         FLOAT_TO_FIX(a),
                         FLOAT_TO_FIX(b),
                         FLOAT_TO_FIX(tol),
                         VERBOSE);
    END_REPEAT_TIMING
    STOP_TIMER(Qn_timer);

    if (iNAN == iResult)
    {
        fprintf(stderr, "Qn %d bisection couldn't find a root. \n", QN);
        exit(99);
    } else
    {
        fprintf(stderr, "Found the root %f using Qn %d bisection.\n",
                FIX_TO_FLOAT(iResult), QN);
    }

    /************************************************************************
     Run the floating point format test 
    ************************************************************************/
    DECLARE_REPEAT_VAR(Floating_timer);
    BEGIN_REPEAT_TIMING(ITERATIONS, Floating_timer)
    Result = bisection(rPoly, a, b, tol, VERBOSE);
    END_REPEAT_TIMING
    STOP_TIMER(Floating_timer);

    if (NAN == Result)
    {
        fprintf(stderr, "Float bisection couldn't find a root. \n");
        exit(99);
    } else
    {
        fprintf(stderr, "Found the root %f using float bisection.\n",
                Result);
    }

    PRINT_RTIMER(Qn_timer, ITERATIONS);
    PRINT_RTIMER(Floating_timer, ITERATIONS);

    /* Display the percentage error */
    Result = fabs(FIX_TO_FLOAT(iResult) - Result);
    fprintf(stderr, "Qn %d error= %f\n", QN, Result);
    return 0;
} /* End main */



/******************************************************************************
 Purpose: Finds a root of scalar, non-linear function f using the integer 
 bisection  method. a and b make up the initial bracket to start bisecting from.
 Only implement this after you have the floating point function implemented
 
 Where: ifunc1arg f - INTEGER function whose root is to be determined
                      must take a single argument of type long and return
        long a      - initial Qn root bracket guess
        long b      - initial Qn root bracket guess
        long atol   - absolute Qn error termination tolerance,
        int verb    - verbose flag, 1 = TRUE, 0 = FALSE
        
Returns: long - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN                 
******************************************************************************/
long ibisection(ifunc1arg f, long a, long b, long atol, int verb)
{
    long c; /* midpoint */
    long fc; /* the value at the midpoint */
    int i = 0; /* iterator */

    if (a > b)
    {
        // Swap them
        long k = b;
        b = a;
        a = k;
    }

    // Check to be sure the selected starting points bracket a root
    // Use the following error "A and B do not center a root"
    int sa = SIGN(f(a)), sb = SIGN(f(b)), sc;
    if (sa == sb)
    {
        fprintf(stderr, "A and B do not center a root\n");
        return iNAN;
    }

    /* Limit the number of attempts */
    while (i < BI_LIMIT)
    {
        /* Calculate the mid point, evaluate it. */
        c  = (a + b) >> 1;
        fc = f(c);
        sc = SIGN(fc);

        /* Did we find a root? - check the function */
        if ((fc < atol && sc == 1) || ((-fc) < atol && sc == -1))
        {
            return c;
        }

        /* Did we find a root? - check the range */
        // b always greater than a
        assert(b - a >= 0);
        if ((b - a) < atol)
        {
            return c;
        }

        /* Print out verbose status */
        if (verb)
        {
            fprintf(stdout, "iter:%d    a:%f    b:%f    x:%f    "
                            "err:%f    y:%f   "
                            "sc: %d   fc: %8lx -fc: %016lx  tol: %016lx\n",
                    i, FIX_TO_FLOAT(a), FIX_TO_FLOAT(b), FIX_TO_FLOAT(c),
                    FIX_TO_FLOAT(b - c), FIX_TO_FLOAT(fc),
                    sc, fc, -fc, atol);
        }

        /* Pick the next bracket */
        if (sc == sa)
        {
            // Pick the right range
            a = c;
        }
        else
        {
            // Pick the left range
            b = c;
        }

        // Track the iterations
        i++;
    }
    fprintf(stdout, "Bisection could not find a root with %d iterations\n", BI_LIMIT);
    return iNAN;
}

/*---------------------------------------------------------------------------
  This function implements the real polynomial equation to be solved.  
  
  Where:   double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double rPoly(double x)
{
    //x**3-.0001x**2-676X+.0676    
    return (x * (x * (x - .0001) - 676.0) + .0676);
}


/*---------------------------------------------------------------------------
  This function implements the integer polynomial equation to be solved.  
  
  Where:    long x - the value to evaluate
  Returns: long - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
long iPoly(long x)
{
    //x**3-.0001x**2-676X+.0676
    long x2 = MUL_FIX(x, x);

    return MUL_FIX(x2, x)
           - MUL_FIX(FLOAT_TO_FIX(0.0001), x2)
           - MUL_FIX(FLOAT_TO_FIX(676), x)
           + FLOAT_TO_FIX(0.0676);
}
