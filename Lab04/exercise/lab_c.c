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
#include <assert.h>


/*---------------------------------------------------------------------------
  These data structures are used to simulated a very simple linked list
---------------------------------------------------------------------------*/
typedef struct Node
{
    int index;
    struct Node* next;
    char string[40];
} Node;

typedef struct
{
    int nterms;
    Node* front;
} Wrapper;


Wrapper* wrapper_init()
{
    Wrapper* self = malloc(sizeof(Wrapper));
    self->nterms = 0;
    self->front = NULL;

    return self;
}

// Function prototypes
void printList(Wrapper* wrapper_p);

Node* node_init(int idx, const char* string)
{
    Node* self = malloc(sizeof(Node));
    strncpy(self->string, string, sizeof(self->string) - 1);
    self->index = idx;
    self->next = NULL;

    return self;
}

void ListAppend(Wrapper* wrapper_p, const char* string)
{
    Node* iter = wrapper_p->front;
    Node* last = NULL;

    int i = 0;
    while (iter)
    {
        i++;
        last = iter;
        iter = iter->next;
    }

    if (!last)
    {
        wrapper_p->front = node_init(i, string);
    }
    else
    {
        last->next = node_init(i, string);
    }

    wrapper_p->nterms++;
}

void node_free(Node* self)
{
    free(self);
}

void wrapper_free(Wrapper* self)
{
    Node* iter = self->front;
    int i = 0;
    while (iter)
    {
        assert(i == iter->index);
        Node* next = iter->next;
        node_free(iter);
        iter = next;
        i++;
    }

    free(self);
}

int main(int argc, char* argv[])
{
    Wrapper* wrapper_p;

    wrapper_p = wrapper_init();

    // Wire the first node in
    printf("Printing 1 node linked list\n");
    ListAppend(wrapper_p, "one");
    printList(wrapper_p);

    // Wire the second node in
    printf("Printing 2 node linked list\n");
    ListAppend(wrapper_p, "two");
    printList(wrapper_p);

    // Clean up memory
    wrapper_free(wrapper_p);
    wrapper_p = NULL;

    return 0;
}

// Walk the linked list and print the contents 
void printList(Wrapper* wrapper_p)
{
    int i = 0;
    struct Node* iter = wrapper_p->front;
    for (; iter && i < wrapper_p->nterms; iter = iter->next, i++)
    {
        assert(iter->index == i);
        printf("node %d contains the string %s\n", iter->index + 1, iter->string);
    }

    assert(i == wrapper_p->nterms);
    assert(iter == NULL);
}
