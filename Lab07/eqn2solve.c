/*---------------------------------------------------------------------------
  05/08/2017    R. Repka    Make derivative clearer
---------------------------------------------------------------------------*/
#include <math.h>

/*---------------------------------------------------------------------------
  Student framework equations to be solved to help with the root finder 
---------------------------------------------------------------------------*/
#include "rootfinding.h"

/*---------------------------------------------------------------------------
  This function implements the equation to be solved.  
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1(double x)
{
    return 0.76 * x * sin(x * 30.0 / 52.0) * tan(x * 10.0 / 47.0)
           + 2.9 * cos(x + 2.5) * sin(0.39 * (1.5 + x));
}


/*---------------------------------------------------------------------------
  This function implements the first derivative equation, which is calculated
  via any off-line process you wish (e.g  Wolfram Alpha)
  There is NO requirement to programmatically generate the derivative.
  
  Where: double x - the value to evaluate
  Returns: double - the value of the function at the point
  Errors:  none
---------------------------------------------------------------------------*/
double func1Deriv(double x)
{
    // Compute some values that will be reused
    // -O2 should apply these anyway but we put them
    // for readability
    double r_15_26 = (15 * x) / 26;
    double r_10_47 = (10 * x) / 47;
    double sin_r_15_26 = sin(r_15_26);
    double cos_r_10_47 = cos(r_10_47);
    double tan_r_10_47 = tan(r_10_47);

    double x25 = x + 2.5;
    double x39_585 = 0.39 * x + 0.585;

    return -2.9 * sin(x39_585) * sin(x25) +
           1.131 * cos(x39_585) * cos(x25) +
           0.76 * sin_r_15_26 * tan_r_10_47 +
           0.438462 * x * cos(r_15_26) * tan_r_10_47 +
           0.161702 * x * sin_r_15_26 * 1 / (cos_r_10_47 * cos_r_10_47);
}
