/******************************************************************
 * Program to correct the data for the sensor
 * Note: Each student gets unique data, so this exact equation will
 * differ for each students solution
 * Be sure to use Honer's factorization  
 * ***************************************************************/

#include <stdio.h>

/* Runs the data through the fitting line */

int main(void)
{
    int res, real, ideal;

    while (scanf("%d %d", &ideal, &real) != EOF)
    {
        /* Insert your polynomial here, be sure to round properly */
        // f(x) = 77.8123 + 0.0760506x + 0.000211552x^2 + -6.05255e-08x^3
        double temp =
                77.8123 + real *
                          (0.0760506 +
                           real * (0.000211552 - 6.05255e-08 * real));
        temp = real - temp;
        res = (int) (temp >= 0 ? temp + 0.5 : temp - 0.5);
        printf("%d %d\n", ideal, res);
    }
    return 0;
}
