#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
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
void imprimir_rashos(char path[]){
printf("%s\n ",path);
struct Master_Boot_Record uno;
FILE* archivo;
archivo=fopen(path,"r+b");
if(archivo){
void rewind(archivo);
fseek(archivo,sizeof(struct Master_Boot_Record),SEEK_SET);
fread(&uno,sizeof(struct Master_Boot_Record),1,archivo);
fclose(archivo);
printf("datos: fecha:%i/%i/%i \n hora: %i:%i \n tam: %i",uno.dia,uno.mes,uno.anio,uno.hora,uno.min,uno.tamaio_mbr);
}
}
void accion_fdisk_normal(int size,int unit,char path[],int type,int fit,char name[]){printf("normal\n");
FILE* archivo;
archivo=fopen(path,"r+b");
struct Master_Boot_Record prueba;
struct MBR_particion nuevo;
if((size<2000&&unit==0)||(size<2&&unit==1)||(size<2000000&&unit==2)){
printf("error tamaño minimo para una particion es de 2MB");
}else{
if(fit==0){nuevo.fit='W';}else if(fit==1){nuevo.fit='F';}else{nuevo.fit='B';}
if(unit==0){nuevo.size = size*10000;}else if(unit==1){nuevo.size = size*1000000;}else{nuevo.size = size;}
archivo= fopen(path,"r+b");
int hay_error = 0;
if(archivo){
fseek(archivo,0,SEEK_SET);
fread(&prueba,sizeof(struct Master_Boot_Record),1,archivo);
int Bool_extend = 0 ;
struct MBR_particion auxiliar = prueba.particion1;
int ini[4]={0,0,0,0};
int tams[4]={0,0,0,0};
if(auxiliar.status=='A'){
ini[0] = auxiliar.part_ini;
tams[0] = auxiliar.size;
if(auxiliar.type=='E'){
Bool_extend = 1;}}
auxiliar = prueba.particion2;
if(auxiliar.status=='A'){
ini[1] = auxiliar.part_ini;
tams[1] = auxiliar.size;
if(auxiliar.type=='E'){
Bool_extend = 1;}}
auxiliar = prueba.particion3;
if(auxiliar.status=='A'){
ini[2] = auxiliar.part_ini;
tams[2] = auxiliar.size;
if(auxiliar.type=='E'){
Bool_extend = 1;}}
auxiliar = prueba.particion4;
if(auxiliar.status=='A'){
ini[3] = auxiliar.part_ini;
tams[3] = auxiliar.size;
if(auxiliar.type=='E'){
Bool_extend = 1;}}
if(strlen(name)<=16){
strcpy(nuevo.name,name);}else{
hay_error = 3;
}
if(type==0){
nuevo.type ='P';
}else if(type==1){
if(Bool_extend==0){
nuevo.type ='E';}//error = 1 ya hay extendida;
else{ hay_error=1;}
}else{
if(Bool_extend!=0){
nuevo.type ='L';}//error = 2 no hay extendida y se crea una logica;
else{ hay_error=2;}
}
fclose(archivo);
if(hay_error==0){
printf("\nfunciono %s\n",nuevo.name);
}}}
}
void accion_fdisk_add(int add,char path[],int unit,char name[]){printf("Añadir\n");}
void accion_fdisk_del(int Delete,char path[],char name[]){printf("ELiminar\n");}
void accion_rmdisk(char path[]){
strcat(path,".dsk");
FILE* archivo;
archivo=fopen(path,"rb");
if(archivo){
fclose(archivo);
int a = remove(path);
if(a==0){
printf("\neliminado exitosamente\n");
}else{
printf("\n no eliminado\n");
}
}else{
printf("\nel archivo no existe\n");
}
}
void accion_mkdisk(int size,char path[],char nom[],int unit){
if(unit==0&&size<10000||unit==1&&size<10){
printf("tamaño minimo de disco es de 10 MB");}
else{
time_t curtime;
struct tm *loctime;
curtime = time (NULL);
loctime = localtime (&curtime);
strcat(path,nom);
FILE* archivo;
archivo=fopen(path,"a+b");
struct Master_Boot_Record master;
master.anio = loctime->tm_year+1900;
master.mes = loctime->tm_mon+1;
master.dia = loctime->tm_mday;
master.hora = loctime->tm_hour;
master.min = loctime->tm_min;
struct MBR_particion auxiliar;
auxiliar.status ='I';
master.particion1 = auxiliar;
master.particion2 = auxiliar;
master.particion3 = auxiliar;
master.particion4 = auxiliar;
if(unit==1){
int tam = size*1000000;
master.tamaio_mbr = tam;
printf("%i  tam:%i",master.min,master.tamaio_mbr);
char a[1000] ="";
for(int j=0;j<1000;j++){int i=0;
while(i<size){
fwrite (a,1,sizeof(a),archivo);
i++;}}
}
else{
int tama = size*1000;
master.tamaio_mbr = tama;
printf("%i  tam:%i",master.min,master.tamaio_mbr);
char a[1000] ="";
for(int j=0;j<size;j++){
fwrite (a,1,sizeof(a),archivo);}
}
fclose(archivo);
printf("%s",path);
archivo= fopen(path,"r+b");
fseek(archivo,0,SEEK_SET);
fwrite(&master,2,sizeof(master),archivo);
fclose(archivo);
}
}
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
if(cad==NULL){
if(size!=0&&strcmp(path,"")!=0&&strcmp(name,"")!=0){
if(add !=0||Delete!=0){
printf("error comandos extra");
}else{
printf("llega");accion_fdisk_normal(size,unit,path,type,fit,name);}}
else if(add!=0&&strcmp(path,"")!=0&&strcmp(name,"")!=0){if(size!=0||Delete!=0){
printf("error comandos extra");
}else{ accion_fdisk_add(add,path,unit,name);}}
else if(Delete!=0&&strcmp(path,"")!=0&&strcmp(name,"")!=0){if(size!=0||Delete!=0){
printf("error comandos extra");
}else{ accion_fdisk_del(Delete,path,name);}}
else{
printf("\nerror faltan parametros\n");}
}
else{
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\n");
if((otro!= NULL) && (otro[0]!='\0')) {}
else{free(otro); char* otro ="";}
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
fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);
}
else if(strcmp(sec,"+unit")==0){//unidad
if(strcmp(ter,"m")==0){
unit = 1;fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);
} else if(strcmp(ter,"b")==0){unit =2; fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}
else if(strcmp(ter,"k")==0){unit =0; fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}
else{printf("unidad incorrecta");}
}
else if(strcmp(sec,"+type")==0){//tipo
if(strcmp(ter,"e")==0){
type = 1;fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add); } else if(strcmp(ter,"l")==0){type=2;fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}
else if(strcmp(ter,"p")==0){type=0;fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}else{printf("type incorrecto"); }}
else if(strcmp(sec,"+fit")==0){//criterios para asignar espacio
if(strcmp(ter,"ff")==0){
fit = 1; fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);} else if(strcmp(ter,"bf")==0){fit =2;fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}
else if(strcmp(ter,"bf")==0){fit =2;fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}else{printf("ajuste incorrecto");}}
else if(strcmp(sec,"-path")==0){//direccion de carpeta
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\"");
sec = ter1;
FILE* archivo;
archivo=fopen(sec,"r+b");
if(archivo){
int a = fclose(archivo);
if(a ==0){path=sec;
fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}else{printf("no se cerro correctamente");}
 }
else{
printf("error, el archivo no se pudo abrir\n");
}}
else if(strcmp(sec,"-name")==0){//verificar nombre completo :)
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
name = ter2;
fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);
}
else if(strcmp(sec,"+add")==0){// tamaño
int num=0;// metodo para encontrar los numeros
char newnum[20]="";
strncpy(newnum, ter, 20);
num = atoi(newnum);
add = num;
fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);
}
else if(strcmp(sec,"+delete")==0){//unidad
if(strcmp(ter,"fast")==0){
Delete = 1; fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);} else if(strcmp(ter,"full")==0){Delete=2; fue_fdisk(otro,size,unit,path,type,fit,Delete,name,add);}
else{printf("Error metodo de eliminar equivocado");}
}
else{
printf("Error comando mal ingresado");}
}
}
}
void fue_rmdisk(char cad[],char path[], int eliminado){
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\n");
if ((otro!= NULL) && (otro[0] != '\0')) {
}else{otro="";}
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
if(strcmp(sec,"-path")==0){
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
printf("\neliminara: %s",ter2);
accion_rmdisk(ter2);
}else{
printf("extension incorrecta");}
}else{
printf("comando incorrecto");}
}
}
void fue_mkdisk(char cad[], int size/*tamaño*/,int unit /*unidad*/,char path[]/*direccion*/,char nom[]/*nombre*/){
if(strcmp(cad,"")==0){
if(size!=0&&strcmp(path,"")!=0&&strcmp(nom,"")!=0){accion_mkdisk(size,path,nom,unit);}
else{printf("\nerror faltan parametros\n");}
}
else{
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\0");
if ((otro!= NULL) && (otro[0] != '\0')) {

}else{otro="";}
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
fue_mkdisk(otro,size,unit,path,nom);
}
else if(strcmp(sec,"+unit")==0){//unidad
if(strcmp(ter,"m")==0){
unit = 1;}
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
fue_mkdisk(otro,size,unit,path,nom);}else{
printf("extension incorrecta");}
fue_mkdisk(otro,size,unit,path,nom);
}
}
else{
printf("Error comando mal ingresado");}
}
}
void analizar_cadena(char cadena[]){
 char* pch;
   pch = strtok(cadena," ");
   printf("cad:%s\n",pch);
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
   printf("\n entro");
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_fdisk(cadena,0,0,"",0,0,0,"",0);
   }
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
int menu(){
int num=1;
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
   num = 0;
    }
return num;}
void prue(){
imprimir_rashos("/home/sergio/hola/si.dsk");
}
int main()
{
int num =1;
do{
    num=menu();}
    while(num!=0);
    return 0;
}
