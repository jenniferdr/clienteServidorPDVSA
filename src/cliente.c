/* Cliente PDVSA  
 * Simulación de una bomba de gasolina que se encarga
 * de surtir gasolina a sus clientes y cuando tiene 
 * espacio suficiente para llenar nuevamente su tanque 
 * hace una peticion al servidor para llenarlo.
 *
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

#include "funciones.h"

int main(int argc, char *argv[]){

  /* Obtener args de la línea de comandos*/
  char* nombre;  // Nombre de esta Bomba
  int capMax;    // Capacidad Máxima (Litros)
  int inven;     // Inventario actual
  int consumo;   // Consumo promedio (Litros*Minutos)
  char * archivo;
  char* nombres[128];
  char* direcciones[128];
  int puertos[1000];
  int tiempos[1000];

 argumentos_cliente(argc,argv,nombre,&inven,&consumo,&capMax, archivo);
    
 obtener_lista_dns(archivo, nombres,direcciones,puertos);
 /* CABLE PARA LEER EL ARCHIVO*/
 /* nombres[0]= "El_Picoteo";
 nombres[1]= "La_Pala";
 nombres[2]= "hola";
 nombres[3] = NULL;
 direcciones[0]="korba.ldc.usb.ve";
 direcciones[1]="lala.ldc.usb.ve";
 direcciones[2]="piscis.ac.labf.usb.ve";
 puertos[0]=1068;
 puertos[1]=1068;
 puertos[2]=1068;
 */
 // CONNECT CON SERVIDOR PARA PEDIR TIEMPOS
 // PEDIR LOS TIEMPOS!!!!


 tiempos[0] = 6;
 tiempos[1] = 4;
 tiempos[2] = 10;


 void swap(int *a , int *b){
   
   int aux;
   aux = *a;
   *a = *b;
   *b = aux;

}
 void swapLetras(char** a , char** b){
   
   char * aux;
   aux = *a;
   *a = *b;
   *b = aux;

}

 // ORDENAR EL ARREGLO DE TIEMPOS y TODOS LOS DEMAS 
 int i = 0 ;
 int minimo;
 int j;
 
 while (nombres[i]!=NULL){
   minimo = i;
   j = i + 1;
   while (nombres[j]!=NULL){
     if (tiempos[j] < tiempos[minimo]){
       minimo = j;
     }
     j = j +1;   
   }
  
   swap(&tiempos[i],&tiempos[minimo]);
   swapLetras(&nombres[i],&nombres[minimo]);
   swapLetras(&direcciones[i],&direcciones[minimo]);
   //swap(&puertos[i],&puertos[minimo]);
   i=i+1;
 }
 // printf("posision 0 %d %s %s %d",tiempos[0], nombres[0], direcciones[0],puertos[0]);
 
 int sock;
 struct sockaddr_in serv_addr;
 printf("\n nombre%s",nombres[1]);
 /*Crear el socket */
 if((sock= socket(AF_INET,SOCK_STREAM,0))==-1)
    exit(0);
 
 /*Recopilar los datos del servidor en serv_addr*/
 
 
 return 0;


}
