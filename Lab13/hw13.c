//  This fills ram with +3 sequential integers
//  student file
//
//   gcc -g -O0 -std=c99 hw13.c -lpthread -o hw13 -Wall -pedantic
//  valgrind --tool=memcheck --leak-check=yes ./hw13 -f -s
// 02/14/2021  R. Repka    Minor changes


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <getopt.h>
#include <pthread.h>
#include <unistd.h>

#define EN_TIME

#include "Timers.h"
#include "ClassErrors.h"

/*--------------------------------------------------------------------------
  Local data structures and defines 
--------------------------------------------------------------------------*/
// Use the larger data size of the Linux cluster, smaller for
// a typical PC. 
// guarantees divisible by 2/3/4/5/7/8  Note: this number is VERY large
// and MAY cause overflow issues so order of operations will be 
// important.  aka    a*b/c  needs to be written as a*(b/c)  because
// it results in a smaller intermediate value 
#define DATA_SIZE           (136*3*5*7*146*512)
#define VALGRIND_DATA_SIZE  (30*3*5*7*8*1024)


// Maximum number of threads 
#define MAX_THREADS     (8)

// The number of iterations to slow down thread execution
#define DELAY_LOOPS_EXP        (5)

// The percentage rate to update thread progress
#define STATUS_UPDATE_RATE (10)

// Thread information control structure 
struct ThreadData_s
{
    int threadID;      // Contains the thread ID number 0..n
    int segSize;       // The amount of total work for this thread
    // threadID*segSize is the starting index of
    // the data to initialize
    unsigned* dataPtr; // Pointer to the one contagious array buffer
    // that each thread will work on
    int trackStatus;   // Flag to identify if status updates should be reported
    int verbose;       // Flag to indicate if the task should run in verbose mode
};


/* Function prototypes */
void* do_process(struct ThreadData_s* self);

/* Used to control access to the progress counter */
pthread_mutex_t progress_lock;
volatile int progress_counter;


int main(int argc, char* argv[])
{
    /*------------------------------------------------------------------------
      General purpose variables
    ------------------------------------------------------------------------*/
    DECLARE_TIMER(timer);
    START_TIMER(timer);

    time_t wallTime = time(NULL);    // Used to report wall execution time.
    int help = 0, verbose = 0, status = 0, thread_n = 0, fast = 0;

    /*------------------------------------------------------------------------
      Thread process information
    ------------------------------------------------------------------------*/
    progress_counter = 0;
    pthread_mutex_init(&progress_lock, NULL);

    /*------------------------------------------------------------------------
       UI variables with sentential values
    ------------------------------------------------------------------------*/
    const char opt_options[] = "t:svfh";
    struct option long_options[] = {
            {"threads", required_argument, 0, 't'},
            {"status",  no_argument,       0, 's'},
            {"verbose", no_argument,       0, 'v'},
            {"fast",    no_argument,       0, 'f'},

            // Help
            {"help",    no_argument,       0, 'h'},
            {0, 0,                         0, 0}
    };

    int rc;
    int option_index;
    opterr = 1;           /* Enable automatic error reporting */
    while ((rc = getopt_long_only(argc, argv,
                                  opt_options, long_options,
                                  &option_index)) != -1)
    {
        switch (rc)
        {
            case 't':
                thread_n = (int) strtol(optarg, NULL, 0);
                break;
            case 's':
                status = 1;
                break;
            case 'v':
                verbose = 1;
                break;
            case 'f':
                fast = 1;
                break;
            case 'h':
                help = 1;
                break;
            case '?':  /* Handled by the default error handler */
            default:
                help = 1;
                fprintf(stderr, "Internal error: undefined option %0xX\n", rc);
                break;
        }
    }

    /*------------------------------------------------------------------------
      Check for command line syntax errors
    ------------------------------------------------------------------------*/
    if ((optind < argc) || help
        || thread_n < 1 || thread_n > MAX_THREADS)
    {
        fprintf(stderr,
                "This program demonstrates threading performance.\n"
                "usage: %s -t[hreads] num [-s[tatus]] [-f[ast]] [-v[erbose]]\n"
                "Options:\n"
                "  -t -threads num      number of thread from 1 to %d (required)\n"
                "  -s -status           display thread progress (optional)\n"
                "  -v -verbose          verbose output (optional)\n"
                "  -f -fast             short run for Valgrind (optional)\n"
                "  -h -help             display this message\n"
                "eg: %s -t 3 -status\n",
                argv[0], MAX_THREADS, argv[0]
        );

        return help ? 0 : PGM_SYNTAX_ERROR;
    } /* End if error */

    long data_size = fast ? VALGRIND_DATA_SIZE : DATA_SIZE;

    /* Get space for the data */
    unsigned int* matrix = malloc(sizeof(int) * data_size);
    if (!matrix)
    {
        perror("malloc()");
        exit(PGM_INTERNAL_ERROR);
    }

    // Print message before starting the timer
    printf("\nStarting %d threads generating %ld numbers\n\n", thread_n,
           data_size);

    long step_size = data_size / thread_n;
    struct ThreadData_s* thread_info =
            malloc(sizeof(struct ThreadData_s) * thread_n);
    if (!thread_info)
    {
        perror("malloc()");
        exit(PGM_INTERNAL_ERROR);
    }

    pthread_t* threads = malloc(sizeof(pthread_t) * thread_n);
    if (!threads)
    {
        perror("malloc()");
        exit(PGM_INTERNAL_ERROR);
    }

    // Spin up N threads
    for (int i = 0; i < thread_n; i++)
    {
        // Build the thread specific information
        struct ThreadData_s* self = &thread_info[i];
        self->verbose = verbose;
        self->dataPtr = matrix + (i * step_size);
        self->threadID = i;
        self->trackStatus = status;
        self->segSize = (int)step_size;

        // Start the thread
        pthread_create(&threads[i], NULL,
                       (void* (*)(void*)) do_process, self);

        if (verbose)
        {
            fprintf(stdout, "Thread:%d  ID:%ld started\n", i,
                    (unsigned long int) threads[i]);
        }
    }

    /* Print out the progress status */
    if (status)
    {
        while (progress_counter < thread_n * (100 / STATUS_UPDATE_RATE))
        {
            long line_count = (progress_counter) * (step_size / STATUS_UPDATE_RATE);
            long percent_complete = (progress_counter * STATUS_UPDATE_RATE) / thread_n;
            printf("Processed: %ld lines %ld%% complete\n",
                   line_count, percent_complete);
            fflush(stdout);
            sleep(2);
        }
    }

    printf("Processed: %ld lines 100%% complete\n", data_size);
    fflush(stdout);

    /* Wait for all processes to end */
    for (int i = 0; i < thread_n; i++)
    {
        int return_code;
        pthread_join(threads[i], (void**) &return_code);
        if (verbose)
        {
            printf("Task %d join 0, task rc %d\n", i, return_code);
            fflush(stdout);
        }
    } // End threads

    STOP_TIMER(timer);
    PRINT_TIMER(timer);

    wallTime = time(NULL) - wallTime;
    fprintf(stderr, "Total wall time = %d sec\n", (int) wallTime);
    fflush(stderr);

    printf("Verifying results...  ");
    for (long i = 0; i < data_size; i++)
    {
        if (matrix[i] != 3 * i)
        {
            printf("Error inData[%ld]= %d != %ld\n", i, matrix[i], 3 * i);
            fflush(stdout);
            exit(PGM_INTERNAL_ERROR);
        }
    } // End verification
    printf("success\n\n");

    // Clean up
    pthread_mutex_destroy(&progress_lock);
    free(matrix);
    free(threads);
    free(thread_info);

    return 0;
} // End main


/****************************************************************************
  This threading process will initialize parts of a very large array by 3's
  It contains code to SLOW execution down so that status updates can be easily
  seen.  The function prototype is defined by pthread so we MUST use it.
  
  void *do_process(void *data)
  Where: void *data - pointer to some user defined data structure
                      We will use struct ThreadData_s
  Returns: void *   - pointer to some user defined return code structure
                      We will use an integer array
  Errors: none
 
****************************************************************************/
void* do_process(struct ThreadData_s* self)
{
    int i, start = self->threadID * self->segSize;
    int track_size = self->segSize / STATUS_UPDATE_RATE;

    // Print out the thread status
    if (self->verbose)
    {
        fprintf(stdout,
                "Thread:%d  track status:%d  seg size:%dKB  data ptr:%p\n",
                self->threadID, self->trackStatus, self->segSize / 256,
                (void*)self->dataPtr);
        fflush(stdout);
    } // End verbose

    // Process all the data in the portion of the array
    for (i = start; i < start + self->segSize; i++)
    {
        self->dataPtr[i - start] = 3 * i;

        // Slow the CPU
        int delay = 1 << DELAY_LOOPS_EXP;
        while (delay)
        {
            delay--;
        }

        // Track status if required
        if (self->trackStatus && i % track_size == 0)
        {
            pthread_mutex_lock(&progress_lock);
            progress_counter++;
            pthread_mutex_unlock(&progress_lock);

            // Print out the thread status if tracking status
            // Only print verbose when updating progress.
            if (self->verbose)
            {
                fprintf(stdout, "%d", self->threadID);
                fflush(stdout);
            } // End verbose
        }  // End if
    } // End while

    // There might be some status left to update
    if (self->trackStatus)
    {
        pthread_mutex_lock(&progress_lock);
        progress_counter++;
        pthread_mutex_unlock(&progress_lock);
    }

    // Return the task ID number + 10
    return 0;
}
