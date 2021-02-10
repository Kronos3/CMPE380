/****************************************************************************
Lab 2 student file
 
 To enable gdb logging
   gdb <binary>
   set logging file <file>
   set logging overwrite on
   set logging on
   
         gcc -std=c99  -g -O0 lab_d.c -o lab_d
         
 10/05/2019 R. Repka    Initial release
****************************************************************************/
#include <stdio.h> 
#include <stdlib.h> 
#include <string.h>
#include <errno.h>


int main(int argc, char *argv[]) {
  char *text_p;        // do not change

  text_p = malloc(12);

  if (!text_p)
  {
    fprintf(stderr, "Failed to allocate memory [%d]: %s\n", errno, strerror(errno));
    return 1;
  }

  strncpy(text_p, "CMPE380 lab malloc try 1.", 12);
  printf("%s\n", text_p);
  text_p[12] = 0;

  text_p = realloc(text_p, 20);
  if (!text_p)
  {
    fprintf(stderr, "Failed to realloc memory [%d]: %s\n", errno, strerror(errno));
    return 1;
  }

  const char* add_str = "-- Realloc";
  strncat(text_p, add_str, 20 - strlen(text_p));

  printf("%s\n", text_p);

  free(text_p);
  return 0;
}
