/****************************************************************************
 Lab 2 student file
 10/05/2019 R. Repka    Initial release
****************************************************************************/
#include <stdio.h> 
#include <stdint.h> 
#include <string.h>
   
/*---------------------------------------------------------------------------
  Student data structure
---------------------------------------------------------------------------*/
struct bandMember {
   char f_name[256];
   char l_name[256];
   int  year;
};

typedef struct bandMember band;

int main(int argc, char *argv[]) {
  union anumber {
    int32_t integer;
    float floating;
  } aNumber1;

  struct astruct_number {
    int32_t integer;
    float floating;
  };

  aNumber1.integer = 5;
  aNumber1.floating = 6.0;

  printf("Integer: %d\nFloating: %f\n", aNumber1.integer, aNumber1.floating);

  printf("sizeof(union anumber) == %lu\n", sizeof(union anumber));
  printf("sizeof(struct bandMember) == %lu\n", sizeof(struct astruct_number));

  band beatles[] = {
    {"John", "Lennon", 1940},
    {"Paul", "McCartney", 1942},
    {"George", "Harrison", 1943},
    {"Ringo", "Starr", 1940},
  };

  for (int i = 0; i < sizeof(beatles)/sizeof(beatles[0]); i++)
  {
    char buffer[32];
    strncpy(buffer, beatles[i].f_name, 32);

    printf("Beatle %s %s %d\n", buffer, beatles[i].l_name, beatles[i].year);
  }

  char run_on_buffer[1024];
  *run_on_buffer = 0;
  for (int i = 0; i < sizeof(beatles)/sizeof(beatles[0]); i++)
  {
    strncat(run_on_buffer, beatles[i].f_name, 512);
  }

  printf("%s\n", run_on_buffer);

  for (int i = 0; i < sizeof(beatles)/sizeof(beatles[0]); i++)
  {
    if (strncmp(beatles[i].f_name, "Ringo", 256) == 0)
    {
      printf("The Ringo index is %d\n", i);
    }
  }

  return 0;
} 

