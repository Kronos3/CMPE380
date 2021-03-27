/***************************************************************************
  Student framework for root finding algorithms for bisection, newton, and secant.
   
  11/10/2015   R. Repka - Initial version
  12/28/2015 - R. Repka - added integer solution features
  11/12/2019 - R. Repka - Include bisection and newton sample code
***************************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include "ClassErrors.h"
#include "rootfinding.h"


/******************************************************************************
 Purpose: Finds a root of scalar, nonlinear function f using the bisection  
 method. a and b make up the initial bracket to start bisecting from.
 
 Where: func1arg f  - function whose root is to be determined
                      must take a single argument of type double and return
        double a    - initial root bracket guess
        double b    - initial root bracket guess
        double atol - absolute error termination tolerance,
        int verb    - verbose flag, 1 = TRUE, 0 = FALSE
        
Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN                    
******************************************************************************/
double bisection(func1arg f, double a, double b, double atol, int verb)
{
    double c; /* midpoint */
    double fc; /* the value at the midpoint */
    int i = 0; /* iterator */

    // Swap the two values to have b > a always
    if (a > b)
    {
        // Swap them
        double k = b;
        b = a;
        a = k;
    }

    // Check to be sure the selected starting points bracket a root
    // Use the following error "A and B do not center a root"
    int sa = SIGN(f(a)), sc;
    int sb = SIGN(f(b));
    if ((fabs(a-b) < atol) || sa == sb)
    {
        fprintf(stderr, "A and B do not center a root\n");
        return NAN;
    }

    /* Limit the number of attempts */
    while (i < BI_LIMIT)
    {
        /* Calculate the mid point, evaluate it.  Be sure to divide by 2.0 not 2*/
        c  = (a + b) / 2.0;
        fc = f(c);
        sc = SIGN(fc);

        /* Did we find a root? - check the function */
        if (fabs(fc) < atol)
        {
            return c;
        }

        /* Did we find a root? - check the range */
        if (fabs(a - b) < atol)
        {
            return c;
        }

        /* Print out verbose status */
        if (verb)
        {
            fprintf(stdout, "iter:%d    a:%f    b:%f    x:%f    err:%f    y:%f\n",
                    i, a, b, c, fabs(b - c), fc);
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

    if (verb)
    {
        fprintf(stdout, "Bisection could not find a root with %d iterations\n", BI_LIMIT);
    }
    return (NAN);
}



/******************************************************************************
 Purpose: Finds a root of function f using the newton method. x0 is the initial 
 guess,  df is the derivative of function f , Nmax is the maximum number
  of iterations, atol is the tolerance, and verb will turn a verbose print* out

  Where: func1arg f  - function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         func1arg df - derivative of function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         double x0   - initial guess for location of root
         int Nmax    - maximum number of iterations to achieve convergence
         double atol - absolute error convergence tolerance
         int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN      
******************************************************************************/
double newton(func1arg f, func1arg df, double x0, int Nmax, double atol, int verb)
{
    double fx0;        /* Value of f(x0) */
    double dx0;        /* Derivative of f(x0) */
    double x1;        /* New x0 value */
    int i = 0;        /* Iteration */

    if (fabs(f(x0)) < atol)
    {
        return x0;
    }

    while (i < Nmax)
    {
        /* Evaluate the function at the initial point */
        fx0 = f(x0);
        dx0 = df(x0);

        /* If the slope or derivative is close to zero then Newton won't converge */
        if (fabs(dx0) < atol)
        {
            fprintf(stdout, "Newton could NOT converge on a root.\n");
            return (NAN);
        }

        /* Calculate the updated point */
        x1 = x0 - (fx0 / dx0);
        if (fabs(x1 - x0) < atol)
        {
            return x1;
        }

        if (verb)
        {
            fprintf(stdout, "iter:%d	x0:%f	x1:%f	err:%f\n",
                    i, x0, x1, fabs(x1 - x0));
        }
        x0 = x1;
        i++;
    }

    if (verb)
    {
        fprintf(stdout, "Newton could not find a root with %d iterations\n", Nmax);
    }
    return (NAN);
}


/******************************************************************************
 Purpose: Finds a root of function f using the secant method. x0 and x1 are 
 the two initial guesses used to find the first tangent.
 
 Where: func1arg f  - function whose root is to be determined
                       must take single argument of type double and return
                       single value of type double
         double x0   - initial guess for location of root
         double x1   - another initial guess for location of root
         int Nmax    - maximum number of iterations to achieve convergence
         double atol - absolute error convergence tolerance
         int verb    - verbose flag, 1 = TRUE, 0 = FALSE

Returns: double - the root refined to the desired tolerance or NAN
Errors:  prints a message and returns with NAN        
******************************************************************************/
double
secant(func1arg f, double x0, double x1, int Nmax, double atol, int verb)
{
    /**
     * let xn1 = next x value in iteration
               xn = current x value
             xnm1 = previous x value
            f(xn) = current y value (at xn)

        Secant update equation:
            xn1 = xn - (f(xn) (xn - xnm1) / ( f(xn) - f(xnm1) ))

        Secant will iterate until a answer close to zero is found
        We have a maximum iteration so that we can break out of
        an infinite iteration.
     */

    double fxnm1 = f(x0);
    for (uint32_t n = 0; n < Nmax; n++)
    {
        double fxn = f(x1);
        if (fabs(fxn) < atol)
        {
            return x1;
        }

        if (verb)
        {
            fprintf(stdout,"iter:%d	x0:%.6f	x1:%.6f	err:%.6f\n",
                   n, x0, x1, fabs(fxn));
        }

        // Perform next step calculation
        double xn1 = x1 - (fxn * (x1 - x0) / (fxn - fxnm1));
        fxnm1 = fxn;
        x0 = x1;
        x1 = xn1;
    }

    if (verb)
    {
        fprintf(stdout, "Secant could not find a root with %d iterations\n", Nmax);
    }
    return NAN;
}
