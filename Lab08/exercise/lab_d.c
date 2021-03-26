#include <math.h>
#include "Timers.h"

#ifndef REPEAT_NUM
#define REPEAT_NUM 100000000
#endif

double mult_impl(double x)
{
    return 4.4*x*x*x*x - 3.3*x*x*x + 2.2*x*x - 1.1*x + 6.0;
}

double pow_impl(double x)
{
    return 4.4*pow(x, 4) - 3.3*pow(x, 3) + 2.2*pow(x, 2) - 1.1*x + 6.0;
}

double horner_impl(double x)
{
    return 6.0 + x*(-1.1 + x*(2.2 + x*(-3.3 + x*(4.4))));
}

int main(int argc, const char* argv[])
{
    double horner_y, mult_y, pow_y;

    DECLARE_REPEAT_VAR(time)
    BEGIN_REPEAT_TIMING(REPEAT_NUM, time);

    horner_y = horner_impl(1.5);

    END_REPEAT_TIMING
    printf("Horner's performance, ans %f\n", horner_y);
    PRINT_RTIMER(time, REPEAT_NUM)
    printf("\n");
    RESET_TIMER(time)

    BEGIN_REPEAT_TIMING(REPEAT_NUM, time);
    mult_y = mult_impl(1.5);
    END_REPEAT_TIMING
    printf("Mult performance, ans %f\n", mult_y);
    PRINT_RTIMER(time, REPEAT_NUM)
    printf("\n");
    RESET_TIMER(time)

    BEGIN_REPEAT_TIMING(REPEAT_NUM, time);
    pow_y = pow_impl(1.5);
    END_REPEAT_TIMING
    printf("Pow performance, ans %f\n", pow_y);
    PRINT_RTIMER(time, REPEAT_NUM)

    return 0;
}
