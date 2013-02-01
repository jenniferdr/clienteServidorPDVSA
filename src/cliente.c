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

  char nombre[MAX_LONG];  // Nombre de esta Bomba
  int capMax;             // Capacidad Máxima (Litros)
  int inven;              // Inventario actual
  int consumo;            // Consumo promedio (Litros*Minutos)
  char archivo[MAX_LONG]; // Nombre de archivo "DNS"

  // Datos de los servidores
  char* nombres[MAX_SERVERS];
  char* direcciones[MAX_SERVERS];
  int puertos[MAX_SERVERS];
  int tiempos[MAX_SERVERS];


  void swapLetras(char** a , char** b){
   
    char * aux;
    aux = *a;
    *a = *b;
    *b = aux;
  }

  void swap(int *a , int *b){
   
    int aux;
    aux = *a;
    *a = *b;
    *b = aux;
  }
 
  argumentos_cliente(argc,argv,nombre,&inven,&consumo,&capMax,archivo);
 
  obtener_lista_dns(archivo, nombres,direcciones,&puertos[0]);
 
  // CONNECT CON SERVIDORES PARA PEDIR TIEMPOS
  int sock;
  struct sockaddr_in serv_addr;
  struct hostent *he;

  /*Crear el socket */
  if((sock= socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Error al crear el socket");
    exit(-1);
  }

  int k = 0;
  while (direcciones[k]!=NULL){
 
    if( (he=gethostbyname(direcciones[k])) == NULL){
      /*FIX Si no conoce a un servidor que lo saque de la lista ? o finalizar programa*/
      herror("Error al identificar el host");
      exit(-1);
    }
    /*Recopilar los datos del servidor en serv_addr*/
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(puertos[k]); 
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  
    bzero(&(serv_addr.sin_zero),8);
   
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){
      // FIX Si no se puede conectar con un servidor 
      //que lo saque de la lista o finalizar programa ?  
      perror("Error al pedir tiempos \n");
      exit(-1);
    }
   
    int tiempoServi;
    // FIX Validar estas llamadas 
    write(sock,"Tiempo",9);
    read(sock,&tiempoServi,sizeof(int));
   
    printf("Lei los datos %d \n",tiempoServi);
    tiempos[k]=tiempoServi;
    k = k + 1;
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
    swap(&puertos[i],&puertos[minimo]);
    i=i+1;
  }

  // printf("posision 0 %d %s %s %d",tiempos[0], nombres[0], direcciones[0],puertos[0]);

  // Inicio de la simulación 
  k = 0;
  int tiempo=0;
  while (tiempo <= 480){
    if ((capMax-inven)>=38000){
      // Pedir Gasolina
      int sock;
      struct sockaddr_in serv_addr;
      /*Crear el socket */
      if((sock= socket(AF_INET,SOCK_STREAM,0))==-1)
	exit(0);
 
      struct hostent *he;
      /*Colocar aqui el nombre extraido del archivo!!!*/
      if( (he=gethostbyname(direcciones[k])) == NULL){
	/*Pedir gasolina a otro servidor*/
	herror("Error al identificar el host");
	exit(-1);
	/*Se pueden salvar valores para usar despues ¿?*/
      }
      /*Recopilar los datos del servidor en serv_addr*/
      serv_addr.sin_family = AF_INET;
      // FIX
      serv_addr.sin_port = htons(puertos[k]); 
      serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  
      bzero(&(serv_addr.sin_zero),8);
     
      if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){ 
	perror("connect() error\n");
	exit(-1);
      }
      // imprimir en los logs pidiendo gasolina 
      int gasolina;
      write(sock,"gasolina",9);
      read(sock,&gasolina,sizeof(int));
      // poner un numero para no te puedo atender
      if (gasolina == 10){
	// no me puede atender
	k=k+1;
	continue; 

      } else {
	// si me puede atender
	// ver si el tiempo de dormir es de verdad ese
	sleep(tiempos[k]); 
	// escribir en logs me atendio 
    
      }
      printf("Lei los datos %d \n",gasolina);
      return 0;
     
    } else {
      usleep(100000);
      inven= inven - consumo;
      tiempo = tiempo + 1;
    } 

  }
 
  return 0;
}
