/****************************************************************************
 The purpose of this example is to exercise, using pointers
 
 student version 
 
 gcc -O1 -Wall -std=c99 -pedantic -g  lab_c.c -o lab_c
 valgrind --tool=memcheck --leak-check=yes --track-origins=yes ./lab_c
 
 09/09/2019 R. Repka    Minor formatting changes
****************************************************************************/
#include <stdio.h>
#include <stdlib.h>
#include <string.h>


/*---------------------------------------------------------------------------
  These data structures are used to simulated a very simple linked list
---------------------------------------------------------------------------*/
typedef struct Node{
    int index;      
    struct Node *next;        
    char string[40];
} Node;

typedef struct {
    int nterms;      
    Node *front;         
} Wrapper;



// Function prototypes
void printList(Wrapper *wrapper_p);

int main(int argc, char *argv[]) {
   Wrapper *wrapper_p;
   Node *node_p;
   Node *temp_p;
   
   // Wire the first node in 
   
   // Wire the second node in 
   
   // Clean up memory
   
   return 0;
}

// Walk the linked list and print the contents 
void printList(Wrapper *wrapper_p) {

}