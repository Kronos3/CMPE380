/*---------------------------------------------------------------------------
  This prints out argv and env variables 
  student copy
---------------------------------------------------------------------------*/
#include <stdio.h>
#include <string.h>

int main(int argc, char *argv[], char *env[]) {
    int i;
    for (i = 0; i < argc; i++)
    {
        printf("argv[%d] = %s\n", i, argv[i]);
    }
    printf("\nENVIROMENT Vars:\n");

    for (const char** ptr = env; *ptr; ptr++)
    {
        printf("%s\n", *ptr);
    }

    return 0;
}
