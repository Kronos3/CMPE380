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
#define Qn_DIVIDE(A, B, n) ((long int) (((long int)(A)<<n)/(B) ))

// QN math and printing functions
void printBinary(unsigned num, short N);

int floatToFixed(double x, int n);

float fixedToFloat(int x, int n);

#define F_decimal(x) (((x) * (x) * (x))*3.0 - 0.0001*(x)*(x) - 676.0*(x) + 0.0676)
#define F_fractional(x) (((x) * (x) * (x))*3 - (x)*(x)/10000 - 676*(x) + 169/2500)

int F_fixed(double x)
{
    int qn = floatToFixed(x, 18);
    int x2 = Qn_MULTIPLY(qn, qn, 18);

    return Qn_MULTIPLY(Qn_MULTIPLY(x2, qn, 18), floatToFixed(3, 18), 18)
        - Qn_DIVIDE(x2, floatToFixed(10000, 18), 18)
        - Qn_MULTIPLY(qn, floatToFixed(676, 18), 18)
        + floatToFixed(0.0676, 18);
}

/*---------------------------------------------------------------------------
  QN testing framework 
---------------------------------------------------------------------------*/
int main(int argc, char *argv[])
{
    // QN testing variables, add more as necessary
    clock_t time1, time2;
    unsigned i;


    //------------------------------------------------------------------------
    // Examine the division utilities
    //------------------------------------------------------------------------
    printf("\nDivision test\n");
    int32_t qn1 = floatToFixed(-0.0625, 18);
    int32_t qn2 = floatToFixed(-0.0625, 18);
    double fn1 = -0.0625;

    printf("   fnum    qnum1(dec)    qnum2(dec)    "
           "qnum1(float)    qnum2(float)\n");
    while (qn1 && qn2 && fn1)
    {
        qn1 /= 2;
        qn2 = Qn_DIVIDE(qn2, floatToFixed(2.0, 18), 18);
        fn1 /= 2.0;
        printf("%06f   %ld   %ld   %06f  %06f\n",
               fn1, qn1, qn2,
               fixedToFloat(qn1, 18), fixedToFloat(qn2, 18));
    }

    //------------------------------------------------------------------------
    // Examine the multiplication utilities
    //------------------------------------------------------------------------
    printf("\nMultiplication test\n");
    qn1 = floatToFixed(64.125, 18);
    qn2 = floatToFixed(0.755, 18);

    int prod = Qn_MULTIPLY(qn1, qn2, 18);
    printf("Product=%f float qnx product %d dec, "
           "converted back %f float\n",
           64.125 * 0.755, prod, fixedToFloat(prod, 18));

    //------------------------------------------------------------------------
    // Implement floating point scientific equations two different ways
    // using normal fractions and using decimal fractions
    //------------------------------------------------------------------------
    printf("\nComplex calculations test\n");
    double x = 1.0;
    fn1 = F_decimal(1.0);
    double fn2 = F_fractional(1.0);


    //---------------------------------------------------------------------
    // Implement QN scientific equations: x**3-.0001x**2-676X+.0676
    //---------------------------------------------------------------------
    qn1 = F_fixed(x);

    // print all 3 answers
    printf("F_1(x) = %f\n"
           "F_2(x) = %f\n"
           "Q(x) = %f",
           fn1, fn2, fixedToFloat(qn1, 18));


    //------------------------------------------------------------------------
    // Examine the performance
    //------------------------------------------------------------------------
    printf("\nPerformance test\n");

    qn1 = floatToFixed(3.1415, 18);
    qn2 = floatToFixed(-674.9325, 18);
    fn1 = 3.1415;
    fn2 = -674.9325;
    volatile double res_f;
    volatile int res_i;

    // Floating point addition
    time1 = clock();
    for (i = 0; i < LOOP_DELAY; i++)
    {
        // Insert your addition here
        res_f += (fn1 + fn2);
    }
    time1 = clock() - time1;

    // QN addition
    time2 = clock();
    for (i = 0; i < LOOP_DELAY; i++)
    {
        // Insert your addition here
        res_i += (qn1 + qn2);
    }
    time2 = clock() - time2;

    // Print out the clicks and who was faster
    printf("Floating: %d, Fixed: %d, faster: %s\n",
           time1, time2, time2 < time1 ? "fixed" : "floating");

    // Floating multiplication
    res_f = 0;
    res_i = 0;
    time1 = clock();
    for (i = 0; i < LOOP_DELAY; i++)
    {
        // Insert your addition here
        res_f += fn1 + fn2;
    }
    time1 = clock() - time1;

    // QN multiplication
    time2 = clock();
    for (i = 0; i < LOOP_DELAY; i++)
    {
        // Insert your addition here
        res_i += Qn_MULTIPLY(qn1, qn2, 18);
    }
    time2 = clock() - time2;

    // Print out the clicks and who was faster
    printf("Floating: %d, Fixed: %d, faster: %s\n",
           time1, time2, time2 < time1 ? "fixed" : "floating");


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

