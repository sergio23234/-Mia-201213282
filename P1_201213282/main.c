#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <dirent.h>
#include <time.h>
typedef struct Extended_Boot_Record{
char status;// I inactivo o A activo
char fit; //ajuste para asignar espacion
int inicio_E; //bit de inicio
int tama; //tamaño
int sig; //siguiente particion extendida
char nom[16]; //nombre de la particion
}Extended_Boot_Record;
typedef struct MBR_particion{
char status;
char type;
char fit;
int part_ini;
int size;
char name[16];
}MBR_particion;
typedef struct Master_Boot_Record{
int tamaio_mbr;
int dia; int mes; int anio;
int hora; int min;
int mbr_disk_signature;
struct MBR_particion particion[4];
}Master_Boot_Record;
typedef struct Super_bloque{
int s_filesystem_type; //sitema de fichero ext2 o ext3
int s_inodes_count;  // numero de inodos en el Disco duro
int s_blocks_count; // numero de bloques en el Disco duro
int s_free_blocks_count;//bloque libres en el disco duro
int s_free_inodes_count;//inodos libres en el disco duro
int dia_m; int mes_m; int anio_m; //ultima fecha en que fue montado
int dia_u; int mes_u; int anio_u; //ultima fecha en que fue desmontado
int mnt_count; //cuantas veces se ha montado el sistema;
int s_magic; //identificara el sistema de archivos tendra valor 0xEF53-->61267
int s_inodo_size; //tamaño del inodo
int s_block_size; //tamaño del bloque
int s_first_ino; //primer inodo libre;
int s_first_blo; //primer bloque libre;
int s_bm_inodo_start; //inicio del mapa de inodos
int s_bm_block_start; //inicio del mapa de bloques
int s_inodo_start; //inicio de la tabla de inodos
int s_block_start; //inicio de la tabla de bloques
}Super_bloque;
typedef struct Journaling{
char tipo_operacion[20]; //añadir/eliminar/modificar archivo/carpeta
int tipo; //0 archivo y 1 carpeta;
char name[20]; //nombre del archvio
char conte[320]; //contenido
int dia;int mes; int anio; //fecha
char propietario[75]; //propietario del archivo
int permisos; //permisos
}Journaling;
typedef struct T_inodo{
int i_link; //numero de enlaces duros;
int i_uid; // UID del usuario propietario del archivo o carpeta;
int i_gid; // GID del grupo al que pertenece el archivo o carpeta;
int i_size; //tamao del fichero en bytes
int i_dia; int i_mes; int i_ani; //fecha que se leyo el inodo sin modificarlos
int i_diac; int i_mesc; int i_anic; //fecha que se creo el inodo
int m_dia; int m_mes; int m_ani; //fecha de la ultima modificacion del inodo
int i_block[15]; //bloques de inodos 12 Directos y 3 Indirectos
char i_type; //inidica si es archivo a, carpeta c o enlace simbolico e;
int i_perm[9];
}T_inodo;
typedef struct B_content{
char b_name[12]; //nombre carpeta o archivo;
int  b_inodo; //apuntado al inodo;
}B_content;
typedef struct T_bloque{
struct B_content b_contenido[4]; //contenido de apuntadores a inodos
}T_bloque;
typedef struct B_arch{
char b_contenido[64]; //contenido del archivo solo 64 bits por bloque
}B_arch;
typedef struct Indirect_Po{
int b_pointers[16];//bloque inidirecto que apunta a carpeta o archivo
} Indirect_Po;
typedef struct montados{
char path[100];
char name[16];
char id[6];
int num;
struct montados *sig;
}montados;
montados *Ini_mont;
int calcular_n_ext2(int T,int S,int I,int B){
float n1 = T-S;
int dividir= 4+I+3*B;
n1 = n1/dividir;
int n = floor(n1);
return n;
}
int calcular_n_ext3(int T,int S,int I,int B,int J){
float n1 = T-S;
int dividir= 4+I+3*B+J;
n1 = n1/dividir;
int n = floor(n1);
return n;
}
void imprimir_rashos(int tam){
printf("tamaño superbloque:%i\n",sizeof(struct Super_bloque));
printf("tamaño inodo:%i\n",sizeof(struct T_inodo));
printf("tamaño bloque:%i\n",sizeof(struct T_bloque));
printf("tamaño Journaling:%i\n",sizeof(struct Journaling));
int a = calcular_n_ext2(tam,sizeof(struct Super_bloque),sizeof(struct T_inodo),sizeof(struct T_bloque));
int b = calcular_n_ext3(tam,sizeof(struct Super_bloque),sizeof(struct T_inodo),sizeof(struct T_bloque),sizeof(Journaling));
printf("tamaño ext2:%i\ntamaño ext3:%i\n",a,b);
}
void rep_disk(char path[],char name[],char id[]){
montados *auxiliar;
if(Ini_mont!=NULL){
auxiliar = Ini_mont;
char *cad =""; int encontrado=0;
while(auxiliar!=NULL&&encontrado==0){
if(strcmp(id,auxiliar->id)==0){
cad=auxiliar->path;
encontrado=1;
}
auxiliar = auxiliar->sig;
}
if(encontrado==1){
struct Master_Boot_Record principal;
FILE* archivo;
int error=0;
archivo=fopen(cad,"rb");
if(archivo){
fseek(archivo,0,SEEK_SET);
fread(&principal,1,sizeof(struct Master_Boot_Record),archivo);
}else{error=1;}
fclose(archivo);
if(error==0){
char *sec,*ter;
sec= strtok(path,".");
ter= strtok(NULL," ");
char ter1[10] ="";
strcpy(ter1,ter);
strcat(sec,".dot");
char cad_final[10000]="";
char cad_fin0[1000]="";
sprintf(cad_fin0,"digraph struct{\n rankdir=TB;\n node[style=filled,shape=square];\n subgraph cluster_0{\n label=\"disco\";\n");
strcat(cad_final,cad_fin0);
struct MBR_particion aux;
int mayor =0;
for(int i=3;i>=0;i--){
aux=principal.particion[i];
char parti[500]="";
if(aux.status=='A'){
int libre=0;
if(mayor==0){
libre = principal.tamaio_mbr -aux.part_ini-aux.size;
mayor = 1;}
else{
libre = principal.particion[i+1].part_ini-aux.part_ini-aux.size;
}
if(libre>0){
sprintf(parti,"L%i[label=\"Libre\" color=deepskyblue];\n",i);
strcat(cad_final,parti);
}
sprintf(parti,"P%i[label=\"%s\\n tipo::%c\\n S.A::%c\\n\" color=darkorchid4];\n",i,aux.name,aux.type,aux.fit);
strcat(cad_final,parti);
}}
int libre = principal.particion[0].part_ini-sizeof(Master_Boot_Record);
if(libre>0){
sprintf(cad_fin0,"L0[label=\"Libre\" color=deepskyblue];\n");
strcat(cad_final,cad_fin0);
}
sprintf(cad_fin0,"MB[label=\"MBR\" color=deepskyblue];\n}\n}");
strcat(cad_final,cad_fin0);
FILE* n_arch;
n_arch=fopen(sec,"w+");
if(n_arch){
fseek(n_arch,0,SEEK_SET);
int longitud=strlen(cad_final);
fwrite(&cad_final,1,longitud,n_arch);
}
fclose(n_arch);
char cad_gen[100]="";
char prueba1[50]="";
sprintf(cad_gen,"dot %s -O %s.%s -T%s",sec,sec,ter1,ter1);
char cad_gen1[75]="";
sprintf(cad_gen1,"xdg-open %s.%s",sec,ter1);
system(cad_gen);
system(cad_gen1);
}
else{printf("error no se pudo abrir archivo\n");}
}
else{
printf("no se encontro id");
}
}
else{
printf("NO hay unidades montadas\n");

}
}
void rep_mbr(char path[],char name[],char id[]){
montados *auxiliar;
if(Ini_mont!=NULL){
auxiliar = Ini_mont;
char *cad =""; int encontrado=0;
while(auxiliar!=NULL&&encontrado==0){
if(strcmp(id,auxiliar->id)==0){
cad=auxiliar->path;
encontrado=1;
}
auxiliar = auxiliar->sig;
}
if(encontrado==1){
struct Master_Boot_Record principal;
FILE* archivo;
int error=0;
archivo=fopen(cad,"rb");
if(archivo){
fseek(archivo,0,SEEK_SET);
fread(&principal,1,sizeof(struct Master_Boot_Record),archivo);
}else{error=1;}
fclose(archivo);
if(error==0){
char *sec,*ter;
sec= strtok(path,".");
ter= strtok(NULL," ");
char ter1[10] ="";
strcpy(ter1,ter);
char cad_fin0[1000]="";
char cad_fin1[1000]="";
char cad_fin2[1000]="";
char cad_fin3[1000]="";
char cad_fin4[1000]="";
char final_fin[100000]="";
sprintf(cad_fin0,"digraph struct{\n node [shape=plaintext]\n matriz [label=<\n <TABLE BORDER=\"0\" CELLBORDER=\"1\" CELLSPACING=\"0\" CELLPADDING=\"4\">\n");
strcat(final_fin,cad_fin0);
sprintf(cad_fin1,"<TR>\n<TD>Nombre</TD>\n<TD>Valor</TD>\n</TR>\n");
strcat(final_fin,cad_fin1);
sprintf(cad_fin2,"<TR>\n<TD>mbr_tamaño</TD>\n<TD>%i</TD>\n</TR>\n",principal.tamaio_mbr);
strcat(final_fin,cad_fin2);
sprintf(cad_fin3,"<TR>\n<TD>mbr_fecha_creacion</TD>\n<TD>%i/%i/%i %i:%i</TD>\n</TR>\n",principal.dia,principal.mes,principal.anio,principal.hora,principal.min);
strcat(final_fin,cad_fin3);
sprintf(cad_fin4,"<TR>\n<TD>mbr_disk_signature</TD>\n<TD>%i</TD>\n</TR>\n",principal.mbr_disk_signature);
strcat(final_fin,cad_fin4);
struct MBR_particion aux;
for(int i=0;i<4;i++){
aux = principal.particion[i];
char cad_fin[1000]="";
int j = i+1;
if(aux.status=='A'){
sprintf(cad_fin,"<TR>\n<TD>part_status_%i</TD>\n<TD>%c</TD>\n</TR>\n",j,aux.status);
strcat(final_fin,cad_fin);
sprintf(cad_fin,"<TR>\n<TD>part_type_%i</TD>\n<TD>%c</TD>\n</TR>\n",j,aux.type);
strcat(final_fin,cad_fin);
sprintf(cad_fin,"<TR>\n<TD>part_fit_%i</TD>\n<TD>%c</TD>\n</TR>\n",j,aux.fit);
strcat(final_fin,cad_fin);
sprintf(cad_fin,"<TR>\n<TD>part_size_%i</TD>\n<TD>%i</TD>\n</TR>\n",j,aux.size);
strcat(final_fin,cad_fin);
sprintf(cad_fin,"<TR>\n<TD>part_start_%i</TD>\n<TD>%i</TD>\n</TR>\n",j,aux.part_ini);
strcat(final_fin,cad_fin);
sprintf(cad_fin,"<TR>\n<TD>part_name_%i</TD>\n<TD>%s</TD>\n</TR>\n",j,aux.name);
strcat(final_fin,cad_fin);
}
}
char cad_fin5[1000]="";
sprintf(cad_fin5,"</TABLE>>];label=\"Nombre\"}");
strcat(final_fin,cad_fin5);
strcat(sec,".dot");
FILE* n_arch;
n_arch=fopen(sec,"w+");
if(n_arch){
fseek(n_arch,0,SEEK_SET);
int longitud=strlen(final_fin);
fwrite(&final_fin,1,longitud,n_arch);
}
fclose(n_arch);
char cad_gen[100]="";
sprintf(cad_gen,"dot %s -O %s.%s -T%s",sec,sec,ter1,ter1);
char cad_gen1[75]="";
sprintf(cad_gen1,"xdg-open %s.%s",sec,ter1);
system(cad_gen);
system(cad_gen1);
}
else{printf("error no se pudo abrir archivo\n");}
}
else{
printf("no se encontro id");
}
}
else{
printf("NO hay unidades montadas\n");
}
}
void accion_umount(char cad[]){
char *pri,*sec; int fin =0;
printf("cadena:%s\n",cad);
while(fin==0){
pri = strtok(cad,";");
sec= strtok(NULL,"\0");
printf("dato:%s\n",pri);
if ((sec!= NULL) && (sec[0] != '\0')) {
cad = sec;
}else{fin=4;}
if(Ini_mont!=NULL){
if(strcmp(Ini_mont->id,pri)==0&&Ini_mont->sig==NULL){
Ini_mont == NULL;
}
else if(strcmp(Ini_mont->id,pri)!=0&&Ini_mont->sig==NULL){
printf("disco no encontrado\n"); fin=5;
}
else if(strcmp(Ini_mont->id,pri)==0&&Ini_mont->sig!=NULL){
montados * aux = Ini_mont;
montados * aux1 = Ini_mont;
aux= aux->sig;
Ini_mont = aux;
aux1->sig = NULL;
free(aux1);
printf("Desmontando exitosamente\n");
}
else{
montados * aux = Ini_mont;
montados * aux1 = Ini_mont;
while(aux1->sig!=NULL){
aux1 = aux1->sig;
if(strcmp(aux1->id,pri)==0){
montados* aux2 = aux1->sig;
aux->sig = aux2;
aux1->sig = NULL;
free(aux1);
printf("Desmontando exitosamente\n");
}
else{
aux = aux1;}
}
}}else{
fin==-1;
}
}
if(fin<0){
printf("No hay disco montado");
}
}
void solo_mount(){
if(Ini_mont!=NULL){
struct montados *aux = Ini_mont;
while(aux!=NULL){
printf("-id:%s -path:%s -nombre:%s\n",aux->id,aux->path,aux->name);
aux = aux->sig;
}
}
else{
printf("No hay particiones montadas\n");
}
}
void accion_mount(char path[],char name[]){
 struct Master_Boot_Record principal;
 struct Extended_Boot_Record primero;
 FILE* archivo;
 int si_lo_encontro=0;
archivo=fopen(path,"rb");
  if(archivo){
fseek(archivo,0,SEEK_SET);
fread(&principal,1,sizeof(struct Master_Boot_Record),archivo);
struct MBR_particion auxiliar;
for(int i =0; i<4&&si_lo_encontro==0;i++){
auxiliar = principal.particion[i];
if(strcmp(auxiliar.name,name)==0){
si_lo_encontro=1;}
if(auxiliar.type=='E'){
fseek(archivo,auxiliar.part_ini,SEEK_SET);
fread(&primero,1,sizeof(struct Extended_Boot_Record),archivo);
if(primero.status=='A'){
if(strcmp(primero.nom,name)==0){
si_lo_encontro = 1;
}
else{
int existe = 0;
while(existe==0){
int sig = primero.sig;
if(sig!=-1){
fseek(archivo,sig,SEEK_SET);
fread(&primero,1,sizeof(struct Extended_Boot_Record),archivo);
fprintf("\ndatos:%c, nombre:%s siguiente:%i\n",primero.status,primero.nom,primero.sig);
if(primero.status=='A'){
if(strcmp(primero.nom,name)==0){
si_lo_encontro = 1;
existe =1;}
}else{existe=-12;}
}
else{existe =-12;}
}
}
}
}
}
if(si_lo_encontro!=0){
int mayor =0; int encontrado=0; int error_hay=0;
int fin=1;  char idf[6]="vd";
char *new_nam="";
montados* nuevo =(struct montados*)malloc(sizeof(struct montados));
new_nam = name;
strcpy(nuevo->name,new_nam);
strcpy(nuevo->path,path);
if(Ini_mont!=NULL){
montados* aux = Ini_mont;
do{
if(strcmp(path,aux->path)==0&&strcmp(name,aux->name)!=0){
    mayor=aux->num; fin++; encontrado=1;
    }
else if(strcmp(path,aux->path)==0&&strcmp(name,aux->name)==0){
printf("Error particion ya cargada\n"); error_hay=1;}
else{
if(mayor<aux->num){
    mayor = aux->num;
}
}
if(aux->sig!=NULL){aux=aux->sig;}
}
while(aux->sig!=NULL&&error_hay==0);
if(error_hay==0){
if(encontrado!=0){
nuevo->num = mayor;
int libre= 48+fin;
int libre1=96+mayor;
char pe = (char)libre1;
char ul = (char)libre;
idf[2]=pe;
idf[3]=ul;
strcpy(nuevo->id,idf);
aux->sig = nuevo;
nuevo->sig = NULL;
printf("particion cargada exitosamente\n");
}
else{
nuevo->num=mayor+1;
int libre= 48+fin;
int libre1 = 96+mayor+1;
char pe = (char)libre1;
char ul = (char)libre;
idf[2]=pe;
idf[3]=ul;
strcpy(nuevo->id,idf);
aux->sig = nuevo;
nuevo->sig = NULL;
printf("particion cargada exitosamente\n");
}
}
}
else{
nuevo->num=1;
int libre= 48+fin;
char pe = (char)97;
char ul = (char)libre;
idf[2]=pe;
idf[3]=ul;
strcpy(nuevo->id,idf);
Ini_mont = nuevo;
nuevo->sig = NULL;
printf("particion cargada exitosamente\n");
}

}
else{
printf("nombre de la particion incorrecto\n");
}
}fclose(archivo);
}
int  fdsik_normal_logico(int size,char path[],char fit,char name[]){
struct Master_Boot_Record principal;
 FILE* archivo;
 int posicion = 0;
struct Extended_Boot_Record aux;
struct Extended_Boot_Record nuevo;
struct MBR_particion auxiliar;
archivo=fopen(path,"rb");
fseek(archivo,0,SEEK_SET);
fread(&principal,1,sizeof(struct Master_Boot_Record),archivo);
for(int i =0;i<4;i++){
if(principal.particion[i].type=='E'){
auxiliar = principal.particion[i];
}}
int tama_ext = auxiliar.part_ini+auxiliar.size;
fseek(archivo,auxiliar.part_ini,SEEK_SET);
fread(&aux,1,sizeof(struct Extended_Boot_Record),archivo);
fclose(archivo);
if(aux.status=='I'){
nuevo.inicio_E = auxiliar.part_ini; //bit de inicio asignado
nuevo.tama = size;
int total_logico = nuevo.inicio_E+size;
if(total_logico>tama_ext){
return 1; //error 1 el tamaño excede el tamaño de la particion logica
}
else{
strcpy(nuevo.nom,name);//nombre asignado
nuevo.fit = fit; //tipo de asignacion
nuevo.status ='A';// status
int libre = tama_ext - total_logico;
if(libre<2000000){ //siguiente asignado
nuevo.sig = -1;
}
else{
printf("\nlibre:%i\n",libre);
nuevo.sig=nuevo.inicio_E+nuevo.tama;}
int numero = nuevo.sig;
archivo=fopen(path,"rb+");
fseek(archivo,auxiliar.part_ini,SEEK_SET);
int b =fwrite(&nuevo,sizeof(Extended_Boot_Record),1,archivo);
if(numero!=-1){
printf("tamaño write:%i +sig:%i\n",b,nuevo.sig);
struct Extended_Boot_Record nuevo_extra;
nuevo_extra.status = 'I';
fseek(archivo,numero,SEEK_SET);
fwrite(&nuevo_extra,sizeof(struct Extended_Boot_Record),1,archivo);
}
fclose(archivo);
return 0;
}
}
else{
int encontrado = 0;
archivo=fopen(path,"r+b");
while(encontrado==0){
printf("sig:%i + aux:%c",aux.sig,aux.status);
int bit_ini = aux.sig;
if(bit_ini!=-1){
fseek(archivo,bit_ini,SEEK_SET);
fread(&aux,1,sizeof(struct Extended_Boot_Record),archivo);
if(aux.status=='I'){
aux.inicio_E = bit_ini;
encontrado = 1;
}
}
else{
encontrado = -1;
}
}
if(encontrado>0){
int total_logico = aux.inicio_E+size;
if(total_logico>tama_ext){
fclose(archivo);
return 1; //error 1 el tamaño excede el tamaño de la particion logica
}
else{
aux.tama = size; //tamaño asignado
strcpy(aux.nom,name);//nombre asignado
aux.fit = fit; //tipo de asignacion
aux.status ='A';// status
int libre = tama_ext - total_logico;
if(libre<2000000){ //siguiente asignado
aux.sig = -1;
}
else{aux.sig=total_logico;}
archivo=fopen(path,"r+b");
fseek(archivo,aux.inicio_E,SEEK_SET);
fwrite(&aux,1,sizeof(struct Extended_Boot_Record),archivo);
if(aux.sig!=-1){
struct Extended_Boot_Record nuevo_extra;
nuevo_extra.status = 'I';
void rewind (archivo);
fseek(archivo,aux.sig,SEEK_SET);
fwrite(&nuevo_extra,1,sizeof(struct Extended_Boot_Record),archivo);
}
printf("\nnombre archivo: %s + bit inicio:%i bit siguiente:%i\n",aux.nom,aux.inicio_E,aux.sig);
fclose(archivo);
return 0;
}
}
else{
fclose(archivo);
return 2; //no se pueden crear mas particiones Logicas
}
}

}
void accion_fdisk_normal(int size,int unit,char path[],int type,int fit,char name[]){
FILE* archivo;
int logico =0;
archivo=fopen(path,"r+b");
struct Master_Boot_Record prueba;
struct MBR_particion nuevo;
if((size<2000&&unit==0)||(size<2&&unit==1)||(size<2000000&&unit==2)){
printf("error tamaño minimo para una particion es de 2MB");
}
else{
int es_logica=0;
int hay_error = 0;
if(archivo){
fseek(archivo,0,SEEK_SET);
fread(&prueba,1,sizeof(struct Master_Boot_Record),archivo);
}fclose(archivo);
if(fit==0){nuevo.fit='W';}else if(fit==1){nuevo.fit='F';}else{nuevo.fit='B';}
if(unit==0){nuevo.size = size*1000;}else if(unit==1){nuevo.size = size*1000000;}else{nuevo.size = size;}
int Bool_extend = 0 ;
struct MBR_particion auxiliar;
int ocupadas =0;
for(int i =0; i<4;i++){
auxiliar = prueba.particion[i];
if(auxiliar.status=='A'){
ocupadas++;
if(auxiliar.type=='E'){
Bool_extend = 1;}}}
for(int i =0; i<4;i++){
auxiliar = prueba.particion[i];
if(strcmp(auxiliar.name,name)==0){
hay_error=6;}}
if(strlen(name)<=16){
strcpy(nuevo.name,name);}else{
hay_error = 3;// nombre mayora a 16 caracteres
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
if(ocupadas==0){
if(prueba.tamaio_mbr>=nuevo.size){
nuevo.part_ini = sizeof(struct Master_Boot_Record);
}else{hay_error=4;/*no hay suficiente espacio en el disco */}
}else if (ocupadas ==4&&nuevo.type!='L'){
hay_error = 5; /*NO hay mas particiones disponibles*/
}
else if(nuevo.type!='L'){
int error_espacio = 1; //no hay espacio
for(int i=0;i<ocupadas;i++){
auxiliar = prueba.particion[i];
if(i==0){
int libre = auxiliar.part_ini-sizeof(prueba);
if(libre>nuevo.size){ error_espacio = 0;
nuevo.part_ini = auxiliar.part_ini+auxiliar.size;
}}
else{
struct MBR_particion auxiliar2 = prueba.particion[i+1];
int tam_par = auxiliar.part_ini+auxiliar.size; //tamaño de la particion 1.
int libre = auxiliar2.part_ini-tam_par;
if(libre>nuevo.size){
error_espacio = 0;
nuevo.part_ini = auxiliar.part_ini+auxiliar.size;}}
}
if(error_espacio==1){
auxiliar = prueba.particion[ocupadas-1];
int tam_part = auxiliar.part_ini+auxiliar.size;
int libre = prueba.tamaio_mbr-tam_part;
if(libre>=nuevo.size){
nuevo.part_ini = auxiliar.part_ini+auxiliar.size;
nuevo.status ='A';
prueba.particion[ocupadas] = nuevo;
}
else{
hay_error = 4; /*no hay suficiente espacio en el disco */
}}
}
if(hay_error==0){
if(nuevo.type=='L'){
int a = fdsik_normal_logico(nuevo.size,path,nuevo.fit,nuevo.name);
switch(a){
case 0 : printf("Particion Logica creada exitosamente\n"); break;
case 1 : printf("Tamaño ingresado mayor al tamaño disponible\n"); break;
case 2: printf("No se pueden crear mas particiones logicas\n"); break;
default: printf("NO se que paso \n");
}
}
else{
if(ocupadas==0){
nuevo.status ='A';
prueba.particion[0]=nuevo;
}
else{
for(int i =0;i<ocupadas+1;i++){
auxiliar = prueba.particion[i];
struct MBR_particion aux;
if(nuevo.part_ini<auxiliar.part_ini){
aux = prueba.particion[i];
prueba.particion[i]=nuevo;
nuevo = prueba.particion[i];}}
}
if(nuevo.type=='E'){
struct Extended_Boot_Record logicon;
logicon.status = 'I';
nuevo.status ='A';
archivo= fopen(path,"r+b");
fseek(archivo,0,SEEK_SET);
fwrite(&prueba,1,sizeof(struct Master_Boot_Record),archivo);
void rewind (archivo);
fseek(archivo,nuevo.part_ini,SEEK_SET);
fwrite(&logicon,1,sizeof(struct Extended_Boot_Record),archivo);
fclose(archivo);
printf("\nparticion creada exitosamente\n");
}
else{
nuevo.status ='A';
archivo= fopen(path,"r+b");
void rewind (archivo);
fseek(archivo,0,SEEK_SET);
fwrite(&prueba,1,sizeof(struct Master_Boot_Record),archivo);
fclose(archivo);
printf("\nparticion creada exitosamente\n");}
}
}
else{
switch(hay_error){
case 1: printf("Ya hay particion extendida\n"); break;
case 2: printf("NO hay particion extendida para crear la logica\n"); break;
case 3: printf("Nombre mayor a 16 caracteres\n"); break;
case 4: printf("No hay espacio suficiente en el disco\n"); break;
case 6: printf("Nombre repetido\n"); break;
default: printf("Ya hay 4 particiones en el disco\n");
}}}
}
void accion_fdisk_add(int add,char path[],int unit,char name[]){printf("Añadir\n");}
void accion_fdisk_del(int Delete,char path[],char name[]){
  struct Master_Boot_Record principal;
 FILE* archivo;
archivo=fopen(path,"rb");
  if(archivo){
fseek(archivo,0,SEEK_SET);
fread(&principal,1,sizeof(struct Master_Boot_Record),archivo);
}fclose(archivo);
struct MBR_particion auxiliar;
auxiliar.status ='I';
int part_elim=-1;
for(int i=0;i<4;i++){
if(principal.particion[i].status!='I'){
if(strcmp(principal.particion[i].name,name)==0){
part_elim = i;
}}}
if(part_elim==-1){
printf("\nparticion no encontrada\n");}
else{
for(int i=0;i<3;i++){
if(i>=part_elim){
principal.particion[i]=principal.particion[i+1];
}}
principal.particion[3]=auxiliar;
archivo= fopen(path,"r+b");
fseek(archivo,0,SEEK_SET);
fwrite(&principal,1,sizeof(struct Master_Boot_Record),archivo);
fclose(archivo);
printf("Particion Eliminada\n");}
}
void accion_rmdisk(char path[]){
FILE* archivo;
archivo=fopen(path,"rb");
if(archivo){
fclose(archivo);
printf("desea remover este archivo: S/N___");
char c=getchar();
if((c)=='s'||(c)=='S'){int a = remove(path);
if(a==0){
printf("\n eliminado exitosamente\n");
getchar();
}else{
printf("\n no eliminado\n");}
}
else{printf("\n no eliminado\n");}
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
srand (time(NULL));
int aleatorio = rand()%20+1;
struct Master_Boot_Record master;
master.anio = loctime->tm_year+1900;
master.mes = loctime->tm_mon+1;
master.dia = loctime->tm_mday;
master.hora = loctime->tm_hour;
master.min = loctime->tm_min;
master.mbr_disk_signature=aleatorio;
struct MBR_particion auxiliar;
auxiliar.status ='I';
for(int i =0;i<4;i++){
master.particion[i] = auxiliar;}
if(unit==1){
int tam = size*1000000;
master.tamaio_mbr = tam;
char a[1000] ="";
for(int j=0;j<1000;j++){int i=0;
while(i<size){
fwrite (a,1,sizeof(a),archivo);
i++;}}
}
else{
int tama = size*1000;
master.tamaio_mbr = tama;
char a[1000] ="";
for(int j=0;j<size;j++){
fwrite (a,1,sizeof(a),archivo);}
}
fclose(archivo);
archivo= fopen(path,"r+b");
fseek(archivo,0,SEEK_SET);
fwrite(&master,sizeof(master),1,archivo);
fclose(archivo);
printf("Disco creado exitosamente\n");
}
}
void fue_rep(char cad[],char name[],char path[],char id[]){
if(cad==NULL){
if(strcmp(path,"")!=0&&strcmp(name,"")!=0&&strcmp(id,"")!=0){
printf("total:\n guardar:%s\n reporte:%s\n buscar:%s\n",path,name,id);
if(strcmp(name,"mbr")==0){
 rep_mbr(path,name,id);
}else if(strcmp(name,"disk")==0){
rep_disk(path,name,id);
}
else{
printf("comando incorrecto por el momento");
}
}
else{printf("\nerror faltan parametros\n");}
}
else{
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\0");
if ((otro!= NULL) && (otro[0] != '\0')) {
}else{free(otro); char* otro ="";}
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
DIR *dirp;
 struct dirent *direntp;
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\""); //falta comprobar archivos.
sec = ter1; int dig =0;
char cad11[100] = ""; int i =0;
while(*sec!='\0'){
if(*sec=='?'){
cad11[i]=' ';
}else{
cad11[i]=*sec;
}
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
path=cad11;
fue_rep(otro,name,path,id);
}
else if(strcmp(sec,"-name")==0){//verificar nombre completo :)
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
sec1 = ter2; int i =0;
while(*sec1!='\0'){
if(*sec1=='?'){ter2[i]=' ';}
i++; sec1++;
}
name = ter2;
fue_rep(otro,name,path,id);
}
else if(strcmp(sec,"-id")==0){//verificar id:)
id= ter;
fue_rep(otro,name,path,id);
}
else{printf("error\n");}
}
}
}
void fue_umount(char cad[],int id,char envio[]){
if(cad==NULL){
if(strcmp(envio,"")!=0){accion_umount(envio);}
else{printf("\nerror faltan parametros\n");}
}
else{
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\0");
if ((otro!= NULL) && (otro[0] != '\0')) {
}else{free(otro); char* otro ="";}
char* sec;/*cadena secundaria derivada de la principal*/ char* ter; /*ultima cadena que utilizaremos*/
int ver = 0;
int comprobante = 0;
char* aux = prin;
while(*prin !=32&&comprobante==0){
if(*prin==58){ver++;}
if(ver==2){comprobante = 1;}
prin++;}
prin = aux;
if(comprobante==1){
char *sec1;
sec = strtok(prin,"::");
sec1= strtok(NULL," ");
ter = strtok(sec1,":");
char cadena1[5]="-id";
int nume_c=49+id;
cadena1[3]=(char)nume_c;
if(strcmp(sec,cadena1)==0){
char cadenanueva[100]="";
strcat(cadenanueva,ter);
strcat(cadenanueva,";");
strcat(cadenanueva,envio);
id++;
fue_umount(otro,id,cadenanueva);
}else{printf("error comando incorrecto");}
}}
}
void fue_mount(char cad[],char path[],char name[]){
if(cad==NULL){
if(strcmp(path,"")!=0&&strcmp(name,"")!=0){
accion_mount(path,name);
}else{printf("faltan parametros\n");}
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
if(strcmp(sec,"-path")==0){//direccion de carpeta
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\"");
sec = ter1;
char un[50]=""; int i=0;
strncpy(un,sec,50);
while(*sec!='\0'){
if(*sec=='?'){un[i]=' ';}
sec++;
i++;
}
FILE* archivo;
archivo=fopen(un,"r+b");
if(archivo){
int a = fclose(archivo);
if(a ==0){path=un;
fue_mount(otro,path,name);
}
else{printf("no se cerro correctamente");}
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
sec1 = ter2; int i =0;
while(*sec1!='\0'){
if(*sec1=='?'){ter2[i]=' ';}
i++; sec1++;
}
name = ter2;
fue_mount(otro,path,name);
}
else{
printf("parametro incorrecto\n");
}}}
}
void fue_fdisk(char cad[], int size, int unit, char path[], int type, int fit, int Delete,char name[], int add){
if(cad==NULL){
if(size!=0&&strcmp(path,"")!=0&&strcmp(name,"")!=0){
if(add !=0||Delete!=0){
printf("error comandos extra");
}else{
accion_fdisk_normal(size,unit,path,type,fit,name);}}
else if(add!=0&&strcmp(path,"")!=0&&strcmp(name,"")!=0){if(size!=0||Delete!=0){
printf("error comandos extra");
}else{ accion_fdisk_add(add,path,unit,name);}}
else if(Delete!=0&&strcmp(path,"")!=0&&strcmp(name,"")!=0){if(size!=0||add!=0){
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
char un[50]=""; int i=0;
strncpy(un,sec,50);
while(*sec!='\0'){
if(*sec=='?'){un[i]=' ';}
sec++;
i++;
}
FILE* archivo;
archivo=fopen(un,"rb");
if(archivo){
int a = fclose(archivo);
if(a ==0){path=un;
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
sec1 = ter2; int i =0;
while(*sec1!='\0'){
if(*sec1=='?'){ter2[i]=' ';}
i++; sec1++;
}
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
char ter2[60] ="";
strncpy(ter2,sec2,50);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
sec = ter2;
int i =0;
while(*sec!='\0'){
if(*sec=='?'){
ter2[i]=' ';
}
sec++;
i++;
}
accion_rmdisk(ter2);
}else{
printf("extension incorrecta");}
}else{
printf("comando incorrecto");}
}
}
void fue_mkdisk(char cad[], int size/*tamaño*/,int unit /*unidad*/,char path[]/*direccion*/,char nom[]/*nombre*/){
if(cad==NULL){
if(size!=0&&strcmp(path,"")!=0&&strcmp(nom,"")!=0){accion_mkdisk(size,path,nom,unit);}
else{printf("\nerror faltan parametros\n");}
}
else{
char* prin/*cadena principal que utilizaremas*/; char* otro/*cadena extra que enviarmeos*/;
prin = strtok(cad," ");
otro = strtok(NULL,"\0");
if ((otro!= NULL) && (otro[0] != '\0')) {
}else{free(otro); char* otro ="";}
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
unit = 1;fue_mkdisk(otro,size,unit,path,nom);
}else if(strcmp(ter,"k")==0){fue_mkdisk(otro,size,unit,path,nom);}
else{printf("error unidad incorecta");}
}
else if(strcmp(sec,"-path")==0){//direccion de carpeta
DIR *dirp;
 struct dirent *direntp;
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\""); //falta comprobar archivos.
sec = ter1; int dig =0;
char cad11[100] = ""; int i =0;
while(*sec!='\0'){
if(*sec=='?'){
cad11[i]=' ';
}else{
cad11[i]=*sec;
}
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
 path=cad11;
 fue_mkdisk(otro,size,unit,path,nom);
}
else if(strcmp(sec,"-name")==0){//verificar nombre completo :)
int comp = 0;
char* sec2 = strtok(ter,"\"");
char *ter1 = strtok(NULL,"\"");
char ter2[20] ="";
strncpy(ter2,sec2,20);
sec1 = strtok(sec2,".");
ter1= strtok(NULL," ");
if(strcmp(ter1,"dsk")==0){
sec = ter2;
int i =0;
while(*sec!='\0'){
if(*sec=='?'){
ter2[i]=' ';
}
i++;
sec++;}
nom = ter2;
fue_mkdisk(otro,size,unit,path,nom);}else{
printf("extension incorrecta");}
}
}
else{
printf("Error comando mal ingresado");}
}
}
void analisis_2(char cad[]){
int i =0; int commi =0;
while(cad[i]!='\0'){
if(cad[i]=='\"'){
if(commi==1){commi =0;}else{commi =1;}
}
if(commi==1){
if(cad[i]==' '){cad[i]='?';}
}else{
char a = cad[i];
cad[i]=tolower(a);
}
i++;
}
analizar_cadena(cad);
}
void fue_exec(char cad[]){
FILE* archivo;
archivo=fopen(cad,"r");
char c;
char cadena[300]="";
int i =0;
int apertura=0;
if(archivo){
while((c=getc(archivo))!=EOF){
if(c=='\n'){
if(apertura==0){
printf("cadena:%s\n",cadena);
analisis_2(cadena);
for(int j=i;j>-1;j--){cadena[j]='\0';}
i=0;}
else{apertura=0;}
}
else if(c=='\\'){
apertura = 1;
cadena[i]=' ';
i++;}
else{
cadena[i]=c;
i++;}
}}
fclose(archivo);
}
void es_exec(char cad[],char path[]){
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
if(strcmp(sec,"-path")==0){//direccion de carpeta
char* ter1 = strtok(ter,"\"");
sec1 = strtok(NULL,"\"");
sec = ter1;
char un[50]=""; int i=0;
strncpy(un,sec,50);
while(*sec!='\0'){
if(*sec=='?'){un[i]=' ';}
sec++;
i++;
}
FILE* archivo;
archivo=fopen(un,"r+b");
if(archivo){
int a = fclose(archivo);
if(a ==0){path=un;
fue_exec(path);}else{printf("no se cerro correctamente");}
 }
}
}
}
int analizar_cadena(char cadena[]){
 char* pch; int salir = 1;
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
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_fdisk(cadena,0,0,"",0,0,0,"",0);
   }
   else if(strcmp(pch,"mount")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   if(cadena!=NULL&&cadena[0]!='\0'){
   fue_mount(cadena,"","");
   }else{
    solo_mount();
   }
   }
   else if(strcmp(pch,"umount")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_umount(cadena,0,"");}
   else if(strcmp(pch,"salir")==0){
   salir =0;
   }
   else if(strcmp(pch,"exec")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   es_exec(cadena,"");
   }
   else if(strcmp(pch,"rep")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   fue_rep(cadena,"","","");
   }
   else if(strcmp(pch,"df")==0){
   pch = strtok (NULL,"\n");
   cadena = pch;
   imprimir_rashos(2000000);
   }
   else if(*pch=='#'){}
   else{
        printf("error\n");
   }
  return salir;
}
int menu1(int con_salto,char cad[]){
char cadena[500]="";
    char c; int num=5;
    int existe_cadena = 0;
    int presiono_enter=1;
    int hay_com=0;
    int i =0;
    if(con_salto==1){
    while(presiono_enter!=0){
    if((c = getchar())!='\n'){
    if(c !='\\'){
    if(c=='\"'){
    hay_com++;
    if(hay_com>1){
    hay_com=0;}
    cadena[i]=c;
    }else{
    if(hay_com==1){
    if(c==' '){
    cadena[i]='?';
    }else{cadena[i]=c;}
    }else{
    cadena[i]=tolower(c);}}
      }
    i++;
    existe_cadena = 1;}
    else{presiono_enter=0;}
    }
    presiono_enter =0;
    strcat(cad,cadena);
    }
    if(cad!=""&&cad[0]!='\0'){
    num = analizar_cadena(cad);
    }
    return num;
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
    int hay_com=0;
    while(presiono_enter!=0){
    if((c = getchar())!='\n'){
    if(c !='\\'){
    if(c=='\"'){
    hay_com++;
    if(hay_com>1){
    hay_com=0;}
    cadena[i]=c;
    }else{
    if(hay_com==1){
    if(c==' '){
    cadena[i]='?';
    }else{cadena[i]=c;}
    }else{
    cadena[i]=tolower(c);}}
      }
    else{cadena[i]==' ';
      con_salto=1;
    }
    i++;
    existe_cadena = 1;}
    else{presiono_enter=0;}
    }
    presiono_enter =0;
    num= menu1(con_salto,cadena);
    //analizar_cadena(cadena);

return num;}

int main()
{
int num =1;
Ini_mont = NULL;
do{
    num=menu();}
    while(num!=0);
    return 0;
}
