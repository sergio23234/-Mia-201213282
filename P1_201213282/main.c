#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void fue_mkdisk(char cad[]){
//printf("Resultado: %s\n",cad);
char* part;
part = strtok(cad," ");
printf("veamos: %s",part);
}
void analizar_cadena(char cadena[]){
 printf("Resultado: %s\n",cadena);
 char* pch;
   pch = strtok(cadena," ");
   if(strcmp(pch,"mkdisk")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_mkdisk(cadena);
   }
   else if(strcmp(pch,"rmdisk")==0){
   printf("rmdisk dos");}
   else if(strcmp(pch,"fdisk")==0){
   printf("fdsik tres");}
   else if(strcmp(pch,"mount")==0){
   printf("mount cuantro");}
   else if(strcmp(pch,"umount")==0){
   printf("umount cinco");}
   else{
        printf("error");
   }
}
int main()
{
    printf("introduzca comando \n");
    char cadena[1000]="";
    int i = 0;
    char c;
    int existe_cadena = 0;
    int con_salto = 0;
    int presiono_enter=1;
    while(presiono_enter!=0){
    if((c = getchar())!='\n'){
    if(c==92){
      cadena[i]=' ';
      con_salto = 1;}
    else{ cadena[i]=tolower(c);}
    i++;
    existe_cadena = 1;}
    else{
    if(con_salto==0){presiono_enter=0;}
    else{con_salto=0;}
    }
    }
    if(existe_cadena==1){
    analizar_cadena(cadena);
    }else{
   printf("Error");
    }
    return 0;
}
