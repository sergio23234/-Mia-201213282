#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
struct MBR_particion{
char status;
char type;
char fit;
int part_ini;
int size;
char name[16];
}MBR_particion;
struct Master_Boot_Record{
int tamaio_mbr;
int dia; int mes; int anio;
int hora; int min;
int mbr_disk_signature;
struct MBR_particion particion1;
struct MBR_particion particion2;
struct MBR_particion particion3;
struct MBR_particion particion4;
}Master_Boot_Record;
struct Extended_Boot_Record{
char status;
char fit;
int inicio_E;
int tama;
int sig;
char nom[16];
}Extended_Boot_Record;
void fue_umount(char cad[], char id[]){

}
void fue_mount(char cad[],char path[],char name[]){
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
if(strcmp(sec,"-path")==0){//direccion de carpeta
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\""); //falta comprobar archivos.
}
else if(strcmp(sec,"-name")==0){//verificar nombre completo :)
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
name = ter2;}else{
printf("extension incorrecta");}
}
}
}
void fue_fdisk(char cad[], int size, int unit, char path[], int type, int fit, int Delete,char name[], int add){
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
if(strcmp(sec,"-size")==0){// tamaño
int num=0;// metodo para encontrar los numeros
char newnum[20]="";
strncpy(newnum, ter, 20);
num = atoi(newnum);
size = num;
}
else if(strcmp(sec,"+unit")==0){//unidad
if(strcmp(ter,"m")==0){
unit = 1;} else if(strcmp(ter,"b")==0){unit =2;}
}
else if(strcmp(sec,"+type")==0){//tipo
if(strcmp(ter,"e")==0){
type = 1;} else if(strcmp(ter,"l")==0){type=2;}
}
else if(strcmp(sec,"+fit")==0){//criterios para asignar espacio
if(strcmp(ter,"ff")==0){
fit = 1;} else if(strcmp(ter,"bf")==0){fit =2;}
}
else if(strcmp(sec,"-path")==0){//direccion de carpeta
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\""); //falta comprobar archivos.
}
else if(strcmp(sec,"-name")==0){//verificar nombre completo :)
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
name = ter2;}else{
printf("extension incorrecta");}
}
else if(strcmp(sec,"+add")==0){// tamaño
int num=0;// metodo para encontrar los numeros
char newnum[20]="";
strncpy(newnum, ter, 20);
num = atoi(newnum);
size = num;
}
}
else if(strcmp(sec,"+delete")==0){//unidad
if(strcmp(ter,"fast")==0){
Delete = 1;} else if(strcmp(ter,"full")==0){Delete=2;}
}
else{
printf("Error comando mal ingresado");}
}
void fue_rmdisk(char cad[],char path[], int eliminado){
 printf("Resultado: %s\n",cad);
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\n");
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
printf("\nveamos: %s",ter);
if(strcmp(sec,"-path")==0){
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
printf("\neliminara: %s",ter2);
/* metodo para eliminar el archivo*/
}else{
printf("extension incorrecta");}
}else{
printf("comando incorrecto");}
}
}
void fue_mkdisk(char cad[], int size/*tamaño*/,int unit /*unidad*/,char path[]/*direccion*/,char nom[]/*nombre*/){
if(size!=0&&strcmp(path,"")!=0&&strcmp(nom,"")!=0&&strcmp(cad,NULL)==0){printf("funciona");}
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
if(strcmp(sec,"-size")==0){// tamaño
int num=0;// metodo para encontrar los numeros
char newnum[20]="";
strncpy(newnum, ter, 20);
num = atoi(newnum);
size = num;
printf("\n entro a size");
fue_mkdisk(otro,size,unit,path,nom);
}
else if(strcmp(sec,"+unit")==0){//unidad
if(strcmp(ter,"m")==0){
unit = 1;}
printf("\n entro a unit");
fue_mkdisk(otro,size,unit,path,nom);
}
else if(strcmp(sec,"-path")==0){//direccion de carpeta
DIR *dirp;
 struct dirent *direntp;
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\""); //falta comprobar archivos.
sec = ter1; int dig =0;
char cad11[100] = ""; int i =0;
while(*sec!='\0'){
cad11[i]=*sec;
if(*sec=='/'){
if(dig=!0){
dirp = opendir(cad11);
 if (dirp == NULL){
 mkdir(cad11,0776);
 }
 }
else{dig++;}
}
sec++;
i++;
}
 path=ter1;
 printf("\n entro a path: %s",path);
 fue_mkdisk(otro,size,unit,path,nom);
}
else if(strcmp(sec,"-name")==0){//verificar nombre completo :)
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
nom = ter2;
 printf("\n entro a name: %s",nom);
fue_mkdisk(otro,size,unit,path,nom);}else{
printf("extension incorrecta");}
}
}
else{
printf("Error comando mal ingresado");}
}
void analizar_cadena(char cadena[]){
 char* pch;
   pch = strtok(cadena," ");
   if(strcmp(pch,"mkdisk")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_mkdisk(cadena,0,0/*Kilobytes*/,"","");
   }
   else if(strcmp(pch,"rmdisk")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_rmdisk(cadena,"",0);}
   else if(strcmp(pch,"fdisk")==0){
   printf("fdsik tres");}
   else if(strcmp(pch,"mount")==0){
   printf("mount cuantro");}
   else if(strcmp(pch,"umount")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_umount(cadena,"");}
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
    int temp = 0,temp1=0;
    while(presiono_enter!=0){
    if((c = getchar())!='\n'){
    if(c==92){
      temp = 1; temp1=i;
      con_salto = 1;}
    else{ cadena[i]=tolower(c);
    if(temp==1){cadena[temp1]='\\'; con_salto=0;}
    }
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
