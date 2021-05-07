/*---------------------------------------------------------------------------
  Create a single thread and use pthread_exit()  
  student copy
  
  gcc -g -std=c99 lab14a.c -lpthread -o lab14a
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h> //For sleep().
#include <pthread.h> //For threads

void* mySimpleThread(void*);

// Global variables
int taskRC[3] = {-1, -1, -1};
pthread_mutex_t mut;
volatile int processed = 0;

int main(int argc, char *argv[])
{
    (void) argc;
    (void) argv;

    // Thread data structure
    pthread_t thread_id[3];

    // Pointer to the process return code, used later
    void *rcp;
    int num[3] = {1, 2, 3};

    time_t start = time(NULL);

    pthread_mutex_init(&mut, NULL);

    // Process index id number
    for (int i = 0; i < 3; i++)
    {
        int rc;
        printf("Main: Starting thread...\n");
        // Student should add the thread create call here
        rc = pthread_create(&thread_id[i],
                            NULL, mySimpleThread, &num[i]);
        if (rc)
        {
            printf("Thread failed to start rc= %d\n", rc);
            exit(99);
        } // End if rc
    }

    printf("Main: All threads started...\n");

    // Print status in the main routine
    while (processed < (6 * 3))
    {
        pthread_mutex_lock(&mut);
        printf("Processed: %d.\n", processed);
        pthread_mutex_unlock(&mut);
        sleep(1);
    }

    // Student should add wait for the thread to finish
    // when instructed. Always check the join RC as you
    // may get memory leaks otherwise.
    for (int i = 0; i < 3; i++)
    {
        pthread_join(thread_id[i], &rcp);
        printf("Thread finished with %p\n", rcp);
    }

    printf("Exiting main thread\n");

    time_t end_time = time(NULL);
    printf("Wall time: %ld\n", end_time - start);

    pthread_mutex_destroy(&mut);

    pthread_exit(&taskRC);
}

/*---------------------------------------------------------------------------
  A thread that prints out it's ID number and sleeps in a loop,
  printing status
---------------------------------------------------------------------------*/
void *mySimpleThread(void *num)
{
    for (int i = 0; i < 6; i++)
    {
        printf("     Thread %d, will sleep 1 second. \n", *(int *) num);
        sleep(1);

        pthread_mutex_lock(&mut);
        processed++;
        pthread_mutex_unlock(&mut);
    }
    taskRC[*(int *) num - 1] = 0;

    return (&taskRC);
}
