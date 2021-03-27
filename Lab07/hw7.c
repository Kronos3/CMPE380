/**************************************************************************
  Student frame work.   Add and remove as you see fit.
  
  07/14/2017    R. Repka    Initial release
 * ***********************************************************************/
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>
#include <assert.h>
#include "ClassErrors.h"
#include "rootfinding.h"
#include "Timers.h"

/************************************************************************
  Tests three types of root finding, secant, newton, and bisection,
  based on user input and prints out the timing results.
************************************************************************/
int main(int argc, char *argv[])
{
    /*------------------------------------------------------------------------
       UI variables with sentential values
    ------------------------------------------------------------------------*/
    int verbose = 0;
    enum modes
    {
        UNDEF, BISECT, SECANT, NEWTON, MODE_MAX,
    } mode = UNDEF;
    double tol = 0.0;
    double guess1 = INFINITY;
    double guess2 = INFINITY;
    int help = 0;

    /*------------------------------------------------------------------------
      These variables are used to control the getopt_long_only command line
      parsing utility.
    ------------------------------------------------------------------------*/
    int rc;
    /* getopt_long stores the option index here. */
    int option_index = 0;

    /* This contains the short command line parameters list */
    char *getoptOptions = "vt:bsn1:2:";    /* add lots of stuff here */

    /* This contains the long command line parameter list, it should mostly
      match the short list                                                  */
    struct option long_options[] = {
            /* These options don’t set a flag.
               We distinguish them by their indices. */
            {"verbose",   no_argument,       0, 'v'},
            {"verb",      no_argument,       0, 'v'},
            /* add lots of stuff here */
            {"tolerance", required_argument, 0, 't'},
            {"tol",       required_argument, 0, 't'},

            // Algorithm selection
            {"bisection", no_argument,       0, 'b'},
            {"secant",    no_argument,       0, 's'},
            {"newton",    no_argument,       0, 'n'},

            // First guess
            {"guessa",    required_argument, 0, '1'},
            {"bracket_a", required_argument, 0, '1'},
            {"ba",        required_argument, 0, '1'},
            {"g1",        required_argument, 0, '1'},
            {"ga",        required_argument, 0, '1'},
            {"g",         required_argument, 0, '1'},

            // Second guess
            {"guessb",    required_argument, 0, '2'},
            {"bracket_b", required_argument, 0, '2'},
            {"bb",        required_argument, 0, '2'},
            {"gb",        required_argument, 0, '2'},
            {"g2",        required_argument, 0, '2'},
            {"u",         required_argument, 0, '2'},

            // Help
            {"help",      required_argument, 0, 'h'},

            {0, 0, 0, 0}
    };

    opterr = 1;           /* Enable automatic error reporting */
    while ((rc = getopt_long_only(argc, argv,
                                  getoptOptions, long_options,
                                  &option_index)) != -1)
    {
        //     printf("getopt_long_only() returned ='%c' index = '%d'\n",  rc, option_index);
        /* Detect the end of the options. */
        switch (rc)
        {
            case 'v':                    /* Verbose */
                verbose = 1;
                break;
            case 't':
                tol = strtof(optarg, NULL);
                break;
            case 'b':
                mode = BISECT;
                break;
            case 's':
                mode = SECANT;
                break;
            case 'n':
                mode = NEWTON;
                break;
            case '1':
                guess1 = strtof(optarg, NULL);
                break;
            case '2':
                guess2 = strtof(optarg, NULL);
                break;

            case 'h':
                help = 1;
                break;

            case '?':  /* Handled by the default error handler */
                break;
            default:
                printf("Internal error: undefined option %0xX\n", rc);
                exit(PGM_INTERNAL_ERROR);
        } // End switch
    } /* end while */

    static const
    char *algorithm_names[] = {
            "Undefined",
            "Bisection",
            "Secant",
            "Newton"
    };

    /*------------------------------------------------------------------------
      Check for command line syntax errors
    ------------------------------------------------------------------------*/
    // Validate arguments
    int invalid = 0;
    if (!help) // don't check inputs on help
    {
        if (mode == UNDEF || mode >= MODE_MAX)
        {
            fprintf(stderr, "-bisection, -secant, or -newton must be "
                            "specified\n");
            invalid = 1;
        }

        switch (mode)
        {
            case BISECT:
                if (guess1 == INFINITY || guess2 == INFINITY)
                {
                    fprintf(stderr, "Two guesses must be provided for "
                                    "%s.\n", algorithm_names[mode]);
                    invalid = 1;
                }
                break;
            case NEWTON:
            case SECANT:
                if (guess1 == INFINITY)
                {
                    fprintf(stderr, "Guess 1 must be provided for %s.\n",
                            algorithm_names[mode]);
                    invalid = 1;
                }

                if (guess2 != INFINITY)
                {
                    fprintf(stderr, "Guess 2 cannot be set for %s.\n",
                            algorithm_names[mode]);
                    invalid = 1;
                }
                break;
            default:
                // algorithm not specified, we already had a warning
                invalid = 1;
                break;
        }

        if (fabs(tol) < HW_EPSILON)
        {
            fprintf(stderr, "[WARNING] Tolerance is set to zero.\n");
        }
    }

    if (optind < argc || help || invalid)
    {
        fprintf(stderr,
                "Tests root finding methods\n"
                "usage: hw7 -b[isection] | -s[ecant] | -n[ewton] -t[ol[erance} number\n"
                "          -g[uess1] number   <-g[u]ess2 number   <-verb[ose]> \n"
                "\n"
                "Root-finding algorithms:\n"
                "One of these must be select\n"
                "  -b, -bisection             Solve the function using bisection\n"
                "  -s, -secant                Solve the function using secant\n"
                "  -n, -newton                Solve the function using newton's method\n"
                "\n"
                "Options:\n"
                "  -g[1,2] -guess[ab]         Provide the guesses or brackets that the selected\n"
                "  -bracket_[ab] -b[ab]       algorithm requires. bisection requires 2,\n"
                "                             newton and secant require 1.\n"
                "  -tolerance -tol -t         Set the floating point tolerance when computing"
                "                             the root of the equation.\n"
                "  -verbose -verb -v          Print each iteration while computing the root.\n"
        );
        return (PGM_INTERNAL_ERROR);
    }

    double result;

    DECLARE_REPEAT_VAR(Timer)
    BEGIN_REPEAT_TIMING(TIM_ITER, Timer)
        if (mode == BISECT)
        {
            /* Performs the root finding with bisection */
            result = bisection(func1, guess1, guess2, tol, verbose);
        } else if (mode == SECANT)
        {
            /* Performs the root finding using the secant method */
            guess2 = guess1 * 1.001;
            result = secant(func1, guess1, guess2, 100, tol, verbose);
        } else
        {
            /* Performs the root finding using newtons method */
            result = newton(func1, func1Deriv, guess1, 100, tol, verbose);
        }

    END_REPEAT_TIMING
    PRINT_RTIMER(Timer, TIM_ITER)

    assert(mode < (sizeof(algorithm_names) / sizeof(algorithm_names[0])));
    if (!isnan(result))
    {
        printf("Found the root %f using %s.\n",
               result, algorithm_names[mode]);
    } else
    {
        printf("No root found using %s.\n", algorithm_names[mode]);
    }

    return PGM_SUCCESS;
}
