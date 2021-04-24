/* Applied Programming Examples: sorting.c
 * student copy
 *
 * Uses qsort() to sort an array of random doubles
 *
 */

#include <stdio.h>
#include <stdlib.h> /* for qsort()    */

/* Function prototypes */
int cmpdbl(const void *p1, const void *p2); /* for qsort() */

/* 
 Initialize an array of doubles of size N, with random numbers
 between -50 and 50, sort it and print it 
*/
int main(int argc, char *argv[])
{
    double *array;
    int num;

    // Check the command line entry
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s N\n", argv[0]);
        return 1;
    }

    num = strtol(argv[1], NULL, 0);

    if (num < 2)
    {
        fprintf(stderr, "Num must be at least two!\n");
        return 2;
    }

    // Get the memory

    array = malloc(sizeof(double) * num);
    if (!array)
    {
        perror("malloc()");
        abort();
    }

    // Initialize the array with random data
    for (int i = 0; i < num; ++i)
    {
        array[i] = (rand() % 1001) / 10.0 - 50.0;
    }

    // Sort the data
    qsort(array, num, sizeof(double), cmpdbl);


    // Print the sorted dat
    for (int i = 0; i < num; ++i)
    {
        printf("% 10.1f\n", array[i]);
    }
    printf("\n");

    free(array);
    array = NULL;

    return (0);
}

/*---------------------------------------------------------------------------
  The compare function to use.  Cast p1 and p2 to doubles in this case
---------------------------------------------------------------------------*/
int cmpdbl(const void *p1, const void *p2)
{
    return *(double *) p1 - *(double *) p2;
}
