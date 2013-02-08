/* Cliente PDVSA  
 * Simulación de una bomba de gasolina que se encarga
 * de surtir gasolina a sus clientes y cuando tiene 
 * espacio suficiente para llenar nuevamente su tanque 
 * hace una peticion al servidor para llenarlo.
 *
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

// Revisar que los prints en el log estan como ellos piden 
// Ver el problema del segmentation fault (Creo que ya lo resolvi)
// Poner las validaciones que faltan en llamadas como read y write
#include "funciones.h"

pthread_mutex_t mutex;
int inventario;
int consumo; // Consumo promedio (Litros*Minutos)
FILE *log;

// Hilo encargado de actualizar tiempo e inventario
void *llevar_tiempo(void *arg_tiempo){
  pthread_detach(pthread_self());

  int *tiempo= (int*) arg_tiempo;
  while(*tiempo<480){
    
    usleep(100000);
    *tiempo= *tiempo +1;

    pthread_mutex_lock(&mutex);

    inventario = inventario-consumo;
    if(inventario<0)inventario=0;
    if (inventario==0){fprintf(log,"Tanque vacio: %d minutos \n", *tiempo);}

    pthread_mutex_unlock(&mutex);

    fflush(log);
  }
  pthread_exit(0);
}

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


int main(int argc, char *argv[]){
  
  char nombre[MAX_LONG];  // Nombre de esta Bomba
  int capMax;             // Capacidad Máxima (Litros)
  char archivo[MAX_LONG]; // Nombre de archivo "DNS"

  // Datos de los servidores
  char* nombres[MAX_SERVERS];
  char* direcciones[MAX_SERVERS];
  int puertos[MAX_SERVERS];
  int tiempos[MAX_SERVERS];

  argumentos_cliente(argc,argv,nombre,&inventario,&consumo,&capMax,archivo);
  obtener_lista_dns(archivo, nombres,direcciones,&puertos[0]);
 
  // creacion del archivo log del cliente
  char nombre_log[MAX_LONG];
  sprintf(nombre_log,"log_%s.txt",nombre);
  log = fopen(nombre_log,"w");


  fprintf(log,"Estado inicial %d \n ", inventario);
  int k = 0;
 
   // CONNECTAR CON SERVIDORES PARA PEDIR TIEMPOS
  while ((direcciones[k])!= NULL){
   
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
      printf("Error de conexion al pedir tiempos: servidor %s en el puerto %d \n"
	     ,direcciones[k],puertos[k]);
      tiempos[k] = 500; // Para que sea ignorado de la lista de servidores
      k = k+1;
      continue;
    }
    
    // FIX Validar estas llamadas 
    write(sock,"Tiempo",9);
    read(sock,&tiempos[k],sizeof(int));
  
    k = k + 1;
    shutdown(sock,2);
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

  // Iniciar contador de tiempo 
  pthread_t contador_tiempo;
  int tiempo=0;
  pthread_create(&contador_tiempo,NULL,llevar_tiempo,&tiempo);
 
  /**** INICIO DE LA SIMULACION ****/ 
  int r = 0;
  while (tiempo <= 480){

    if(direcciones[k]==NULL)r=0;

    if ((capMax-inventario)>=38000){
      int sock;
      struct sockaddr_in serv_addr;
      /*Crear el socket */
      if((sock= socket(AF_INET,SOCK_STREAM,0))==-1)
	exit(0);
 
      struct hostent *he;
      if( (he=gethostbyname(direcciones[r])) == NULL){
	/*Pedir gasolina a otro servidor*/
	herror("Error al identificar el host");
	r = r + 1; // FIX Y si te pasas del arreglo ? Segmentation Fault !!
	continue;
      }
      
      /*Recopilar los datos del servidor en serv_addr*/
      serv_addr.sin_family = AF_INET;
      if (tiempos[r]==500){ r = r +1; continue;} // FIX IGUAL ACA
      serv_addr.sin_port = htons(puertos[r]); 
      serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  
      bzero(&(serv_addr.sin_zero),8);
      
      if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){ 
	printf("Error al conectar con el servidor %s", direcciones[r]);
	r = r + 1;  // FIX IGUAl aCa y en todos los r+1 que quedan
	continue;
      }
      
      write(sock,nombre,MAX_LONG);

      char gasolina[20];
      int recibidos;
      if( (recibidos= recv(sock,gasolina,20,0) ==- 1)){
	perror("Error al recibir el mensaje");
	// Hay que ir al siguiente servidor ? r+1 y continue ?
      }

      if (strcmp(gasolina,"noDisponible")==0){
	fprintf(log,"Peticion: %d minutos, %s , No disponible, %d litros \n",
		tiempo, nombres[r],inventario);
	r=r+1;
	continue; 
	
      } else {
	fprintf(log,"Peticion: %d minutos, %s, OK, %d litros  \n",
		tiempo, nombres[r],inventario);
	
	usleep(tiempos[r]*100000); 

	pthread_mutex_lock(&mutex);
	inventario = inventario + 38000;
	pthread_mutex_unlock(&mutex);

	fprintf(log,"Llegada Gandola: %d minutos, %d litros \n", tiempo,inventario);
	if (inventario==capMax){ fprintf(log,"Tanque Full: %d minutos\n",tiempo);}

	// Reiniciar la busqueda de servidores activos
	r = 0;
      } 
    }
  }
  fclose(log);
  
  return 0;
}
