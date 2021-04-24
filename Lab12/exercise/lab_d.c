/* Applied Programming Examples: sorting.c
 * student copy
 *
 * Uses qsort() to sort an array of random polar data
 *
 * Use compiler directive -DN=size to change the size of the array
 *
 * Reference: A. Kelley and I Pohl "A book on C" 4th Ed.
 * Revised: 3/31/2015 (JCCK)
 */

#include <stdio.h>
#include <math.h>
#include <float.h>
#include <stdlib.h> /* for qsort()    */

/* The polar data structure */
typedef struct
{
    double mag;
    double ang;
} polar;

/* Add your polar compare function prototype here */
int compare_polar(const polar* p1, const polar* p2);

/* 
 Initialize an array of doubles of size N, with random numbers
 between -50 and 50, sort it and print it 
*/
int main(int argc, char *argv[])
{
    polar *array;
    int num;
    int i;

    // Check the command line entry
    if (argc != 2)
    {
        fprintf(stderr, "usage: %s N\n", argv[0]);
        return 1;
    }

    num = (int)strtol(argv[1], NULL, 0);

    if (num < 2)
    {
        fprintf(stderr, "Num must be at least two!\n");
        return 2;
    }

    // Get the memory
    array = malloc(sizeof(polar) * num);
    if (!array)
    {
        perror("malloc()");
        abort();
    }

    // Initialize the array with random data, make one magnitude identical
    for (i = 0; i < num - 1; ++i)
    {
        array[i].mag = (rand() % 1001) / 10.0 - 50.0;
        array[i].ang = (rand() % 1001) / 10.0 - 50.0;
    }
    // Identical
    array[i].mag = array[i - 1].mag;
    array[i].ang = (rand() % 1001) / 10.0 - 50.0;

    // Sort the data
    qsort(array, num, sizeof(polar), (__compar_fn_t) compare_polar);

    // Print the sorted dat
    for (i = 0; i < num; ++i)
    {
        printf("%10.1f %10.1f\n", array[i].mag, array[i].ang);
    }
    printf("\n");

    free(array);
    array = NULL;

    return (0);
}

/*---------------------------------------------------------------------------
  The compare function to use.  Cast p1 and p2 to polar in this case
---------------------------------------------------------------------------*/
// add your function and code here
int compare_polar(const polar* p1, const polar* p2)
{
    if (p1->mag == p2->mag)
    {
        return (int)(p1->ang - p2->ang);
    }

    return (int)(p1->mag - p2->mag);
}
