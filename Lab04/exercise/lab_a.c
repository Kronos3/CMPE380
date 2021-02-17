/****************************************************************************
 The purpose of this example is to exercise using data and function pointers.
 It also demonstrates the value of using NULL assignments to identify pointer
 issues.
 
 student version 
 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_b.c -o lab_b
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./lab_b
 
 
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

typedef struct { 
   int entry;
   char string [40];
} Data;

int fun2x(int num);
int fun3x(int num);

Data ramData;


int main(int argc, char *argv[]) {
   int(*function_p)(int);
   int answer;
   Data stackData;
   static Data staticData;
   Data *heapData_p = malloc(sizeof(Data));
   if (!heapData_p)
   {
       fprintf(stderr, "Failed to allocate space for heapData_p: %s\n", strerror(errno));
       return 1;
   }
 
   // Print out the addresses of items
   printf("main: %p\n"
          "staticData: %p\n"
          "ramData: %p\n"
          "heapData_p: %p\n"
          "stackData: %p\n",
          (void*)&main, (void*)&staticData,
          (void*)&ramData,
          (void*)&heapData_p,
          (void*)&stackData);

    // Using function pointers
    function_p = fun2x;
    printf("passing fun2x 2: got %d\n", function_p(2));

    function_p = fun3x;
    printf("passing fun3x 2: got %d\n", function_p(2));

    // Initialize the stack data
    stackData.entry = 1;
    strcpy(stackData.string, "Stack");

    printf("Stack data values before free: %d '%s'\n", stackData.entry, stackData.string);

    // Initialize the heap data, free it and use it
    heapData_p->entry = 2;
    strcpy(heapData_p->string, "Pointer");
    printf("Heap data values before free: %d '%s'\n", heapData_p->entry, heapData_p->string);

    free(heapData_p);
    heapData_p = NULL;
    printf("Heap data values after free: %d '%s'\n", heapData_p->entry, heapData_p->string);

    return 0;
}


int fun2x(int num) {
   return(num*2);
}
int fun3x(int num) {
   return(num*3);
}
