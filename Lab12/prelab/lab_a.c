/*--------------------------------------------------------------------------
  Simple scanf test program
  student copy
--------------------------------------------------------------------------*/
#include <stdio.h>                    /* ex2a_scan.c */

const double mi2km = 1.609;
double convert(double mi) {return (mi * mi2km);}
int main() {
    double miles;
    printf("Miles to Km Conversion (enter negative number to end)\n");
    do {
        printf("Input distance in miles: ");
        // Your code here
        if (scanf("%lf", &miles) > 0)
        {
            printf("\n%f miles = %f km\n", miles, convert(miles));
        }
        else
        {
            fprintf(stderr, "Failed to parse string\n");
            return 1;
        }
    } while (miles > 0.0);
    return 0;
}
