/*---------------------------------------------------------------------------
  QN code testing and verification
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>
#include <float.h>
#include <time.h>
#include <math.h>

#define LOOP_DELAY (1<<27)

// QN math functions which pass the number of bits "n" 
#define Qn_MULTIPLY(A, B, n) ((A>>(n/2))*(B >>(n-(n/2))))
#define Qn_DIVIDE(A, B, n) ((int) (((long int)(A)<<n)/(B) ))

// QN math and printing functions
void printBinary(unsigned num, short N);

int floatToFixed(double x, int n);

float fixedToFloat(int x, int n);

/*---------------------------------------------------------------------------
  QN testing framework 
---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // QN testing variables, add more as necessary
    double fnum1, fnum2;
    int qnum1, qnval;

    //------------------------------------------------------------------------
    // Print data on sizes
    //------------------------------------------------------------------------
    printf("Integers are %ld bits long\n", 8 * sizeof(int));

    //------------------------------------------------------------------------
    // Prove floating point has limits
    //------------------------------------------------------------------------

    printf("10.0 != 10.0 + DBL_EPSILON => %d\n", 10.0 != 10.0 + DBL_EPSILON);

    //------------------------------------------------------------------------
    // Examine the conversion utilities
    //------------------------------------------------------------------------
    printf("\nConversion test\n");
    struct
    {
        double original;
        int n;
    }conversion[] = {
            {0.0, 0},
            {12.25, 3},
            {12.0625, 3},
            {12.0625, 4}
    };

    for (int i = 0; i < sizeof(conversion) / sizeof(conversion[0]); i++)
    {
        int converted = floatToFixed(conversion[i].original, conversion[i].n);
        float back = fixedToFloat(converted, conversion[i].n);
        printf("Started with %f converted to qnx = %d decimal then back to %f\n",
               conversion[i].original, converted, back);
        printf("QN %d decimal in binary is ", converted);
        printBinary(converted, 16);
    }

    return (0);
}


/*---------------------------------------------------------------------------
  This prints a number in character binary bracketed by [..]
  
  Where: unsigned num - number to print
         short N      - number of bits to print
         
  Returns: nothing
  Error handling: none
----------------------------------------------------------------------------*/
void printBinary(unsigned num, short N)
{
    unsigned i;
    printf("\t[");

    for (i = 1 << (N - 1); i > 0; i = i >> 1)
    {
        (num & i) ? printf("1") : printf("0");
    }
    printf("]\n");
    return;
}


/*---------------------------------------------------------------------------
  This convert from Float to FP(Qn) 
  
  Where: double x  - number to convert
         int qnval - number of bits to use
         
  Returns: int    - QN encoded number
  Error handling: none
----------------------------------------------------------------------------*/
int floatToFixed(double x, int qnval)
{
    return (x * (double) (1 << qnval));
}

/*---------------------------------------------------------------------------
  This converts from FP(Qn) to Float
  
  Where: int x     - number to convert
         int qnval - number of bits to use
         
  Returns: float   - converted number
  Error handling: none
----------------------------------------------------------------------------*/
float fixedToFloat(int x, int qnval)
{
    return ((double) x / (double) (1 << qnval));
}

