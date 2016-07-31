#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main()
{
    printf("introduzca comando \n");
    char *cadena;
    char c;
    while( (c = getchar())!= '\n' ){
    putchar(c);

    }
    return 0;
}
