#include <stdio.h>
#include <stdlib.h>
#include <string.h>
void fue_mkdisk(char cad[], int size/*tamaño*/,int unit /*unidad*/,char path[]/*direccion*/,char nom[]/*nombre*/){
//printf("Resultado: %s\n",cad);
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\n");
printf("\nveamos: %s",otro);
char* sec;/*cadena secundaria derivada de la principal*/ char* ter; /*ultima cadena que utilizaremos*/
int ver = 0;
int comprobante = 0;
char* aux = prin;
while(*prin !=32&&comprobante==0){
if(*prin==58){ver++;}
if(ver==2){comprobante = 1;}
prin++;}
prin = aux;
if(comprobante==1){ char *sec1;
sec = strtok(prin,"::");
sec1= strtok(NULL," ");
ter = strtok(sec1,":");
if(strcmp(sec,"-size")==0){
int num=0;
num = atoi(&ter);
printf("\n%d y otro:%s",num,ter);
}else if(strcmp(sec,"+unit")==0){}
else if(strcmp(sec,"-path")==0){}
else if(strcmp(sec,"-name")==0){}
else{
printf("Error");}
}
else{
printf("Error");
}

}
void analizar_cadena(char cadena[]){
 printf("Resultado: %s\n",cadena);
 char* pch;
   pch = strtok(cadena," ");
   if(strcmp(pch,"mkdisk")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_mkdisk(cadena,0,0/*Kilobytes*/,"","");
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
