#include <stdio.h>
#include <stdlib.h>
#include <string.h>

void analizar_cadena(char cadena[]){
printf("Resultado: \n%s",cadena);

}
int main()
{
    printf("introduzca comando \n");
    char cadena[1000];
    int i = 0;
    char c;
    int a = 0;
    while( (c = getchar())!='\n'){
    cadena[i]=tolower(c);
    i++;
    a = 1;
    }
    if(a==1){
    analizar_cadena(&cadena);
    }else{
   printf("Error");
    }
    return 0;
}
