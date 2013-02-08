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
  FILE *log;
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
 
  // creacion del archivo log del cliente
  char nombre_log[MAX_LONG];
  sprintf(nombre_log,"log_%s.txt",nombre);
  log = fopen(nombre_log,"w");


  fprintf(log,"Estado inicial %d \n ", inven);
  int k = 0;
 
  while ((direcciones[k])!= NULL){
   
    // CONNECT CON SERVIDORES PARA PEDIR TIEMPOS
    int sock;
    struct sockaddr_in serv_addr;
    struct hostent *he;
    
    /*Crear el socket */
    if((sock= socket(AF_INET,SOCK_STREAM,0))==-1){
      perror("Error al crear el socket");
      exit(-1);
    }

    if((he=gethostbyname(direcciones[k])) == NULL){
      
      perror("Error al identificar el host");
      tiempos[k] = 500; // Para que sea ignorado de la lista de servidores
      
      k=k+1;
      continue;

    }
    /*Recopilar los datos del servidor en serv_addr*/
    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(puertos[k]); 
   
    serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  
    bzero(&(serv_addr.sin_zero),8);
   
    if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){
      printf("Error al pedir tiempos \n del servidor %s en el puerto %d",direcciones[k],puertos[k]);
      perror("ERROR EN CONEXION");
      tiempos[k] = 500; // Para que sea ignorado de la lista de servidores
      k = k+1;
      continue;
    }
    
    int tiempoServi;
    // FIX Validar estas llamadas 
  
    write(sock,"Tiempo",9);
    read(sock,&tiempoServi,sizeof(int));
  
    //   printf("Lei los datos %d \n",tiempoServi);
    tiempos[k] = tiempoServi;
    k = k + 1;
    shutdown(sock,2);
    //  printf("Termine iteracion %d \n",k-1);
  }
 
  // ORDENAR EL ARREGLO DE TIEMPOS y TODOS LOS DEMAS 
  int i = 0 ;
  int minimo;
  int j;
  printf("Hola voy a ordenar \n");

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
   printf("posicion 2 %d %d\n", tiempos[2],puertos[2]);
 
  // Inicio de la simulación 
  int r = 0;
  int tiempo = 0;
  while (tiempo <= 480){

    if (inven==0){fprintf(log,"Tanque vacio: %d minutos \n", tiempo);}
    if (inven==capMax){ fprintf(log,"Tanque Full: %d minutos\n",tiempo);}
    fflush(log);

    if ((capMax-inven)>=38000){
      int sock;
      struct sockaddr_in serv_addr;
      /*Crear el socket */
      if((sock= socket(AF_INET,SOCK_STREAM,0))==-1)
	exit(0);
 
      struct hostent *he;
      /*Colocar aqui el nombre extraido del archivo!!!*/
      if( (he=gethostbyname(direcciones[r])) == NULL){
	/*Pedir gasolina a otro servidor*/
	herror("Error al identificar el host");
	//	exit(-1);
	/*Se pueden salvar valores para usar despues ¿?*/
	r = r + 1;
	continue;
      }
      
      /*Recopilar los datos del servidor en serv_addr*/
      serv_addr.sin_family = AF_INET;
      // FIX
      if (tiempos[r]==500){ r = r +1; continue;} 
      serv_addr.sin_port = htons(puertos[r]); 
      serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  
      bzero(&(serv_addr.sin_zero),8);
      
      if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){ 
	perror("connect() error\n");
	// exit(-1);
	r = r + 1;
	continue;
	
      }

    
      // write(sock,nombre,sizeof(char)*14);
      
      write(sock,nombre,MAX_LONG);
      char gasolina[20];
      int recibidos;
      if( (recibidos= recv(sock,gasolina,20,0) < 20)){
	perror("Error al recibir el mensaje");
       }
     
      
      // read(sock,gasolina,sizeof(char)*14);
      printf("lo que recibo%s  ", gasolina);
      // poner un numero para no te puedo atender
      if (gasolina == "noDisponible"){
	fprintf(log,"Peticion: Tiempo %d, Nombre Centro %s , Nodisponible\n", tiempo, nombres[r]);
	r=r+1;
	continue; 
	
      } else {
	  // ver si el tiempo de dormir es de verdad ese
	fprintf(log,"Peticion: Tiempo %d, Nombre Centro %s ,Disponible\n", tiempo, nombres[r]);
	
	sleep(tiempos[r]); 
	inven = inven + 38000; 
	fprintf(log,"Llegada Gandola : tiempo %d inventario %d\n", tiempo,inven);
 	}
      tiempo = tiempo +1;
      //printf("Lei los datos %s \n",gasolina);
      //  return 0;
      
    } else {
      
      printf("tiempo es %d",tiempo);
      
      usleep(100000);
      inven = inven - consumo;
      tiempo = tiempo + 1;
      r = 0 ;
    } 
    
  }
  fclose(log);
  
  return 0;
}
