/* Cliente PDVSA  
 * Simulación de una bomba de gasolina que se encarga
 * de surtir gasolina a sus clientes y cuando tiene 
 * espacio suficiente para llenar nuevamente su tanque 
 * hace una peticion al servidor para llenarlo.
 *
 * Version 2.0 (Implementacion RPC)
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

#include "servicioPDVSA.h"
#include "funciones.h"
#include <pthread.h>


pthread_mutex_t mutex; // mutex para la variable inventario
int inventario;
int consumo; // Consumo promedio (Litros*Minutos)
FILE *LOG;

// Hilo encargado de actualizar tiempo e inventario
void *llevar_tiempo(void *arg_tiempo){
  pthread_detach(pthread_self());
  
  int *tiempo= (int*) arg_tiempo;
  while(*tiempo<=480){
    
    usleep(100000);
    *tiempo= *tiempo +1;
    
    // Actualizar inventario
    pthread_mutex_lock(&mutex); // Seccion critica ==> Inventario
    if(inventario!=0){
      inventario = inventario-consumo;
      if(inventario<0)inventario=0;
      if (inventario==0){fprintf(LOG,"Tanque vacio: %d minutos \n", *tiempo);}
    }
    pthread_mutex_unlock(&mutex);

    fflush(LOG);
  }
  pthread_exit(0);
}


int main(int argc, char *argv[]){
  
  char nombre[MAX_LONG];  // Nombre de esta Bomba
  char *nombre_pointer= &nombre[0]; // para poder enviarlo por RPC 
  int capMax;             // Capacidad Máxima (Litros)
  char archivo[MAX_LONG]; // Nombre de archivo "DNS"

  // Datos de los servidores
  char* nombres[MAX_SERVERS];
  char* direcciones[MAX_SERVERS];
  int tiempos[MAX_SERVERS]; //tiempos de respuesta
  CLIENT *clnts[MAX_SERVERS];

  // Validar y obtener argumentos del cliente
  argumentos_cliente(argc,argv,nombre,&inventario,&consumo,&capMax,archivo);

  obtener_lista_dns(archivo, nombres,direcciones);
 
  // creacion del archivo LOG del cliente
  char nombre_LOG[MAX_LONG];
  sprintf(nombre_LOG,"log_%s.txt",nombre);
  LOG = fopen(nombre_LOG,"w");

  fprintf(LOG,"Inventario inicial %d \n ", inventario);
  if(inventario == 0) fprintf(LOG,"Tanque vacio: 0 minutos \n");
  if(inventario == capMax) fprintf(LOG,"Tanque full: 0 minutos \n");
  
  // PEDIR TIEMPOS
  int k = 0;
  while ((direcciones[k]) != NULL){

    clnts[k]= clnt_create (direcciones[k], SERVICIOPDVSA, SERVICIOPDVSAVERS, "tcp");
    if(clnts[k] == NULL){
      clnt_pcreateerror( direcciones[k] );
      tiempos[k] = 500;
      k = k + 1;
      continue;
    }
     
    int *result = pedir_tiempo_1(NULL,clnts[k]);
    if ( result == (int *)NULL){
      clnt_perror( clnts[k], "Error al conectar con servidor");
      tiempos[k] = 500;
    }else{
      tiempos[k]= *result;
    }
    
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
    i=i+1;
  }
 

  // Iniciar contador de tiempo 
  pthread_t contador_tiempo;
  int tiempo=0;
  pthread_create(&contador_tiempo,NULL,llevar_tiempo,&tiempo);
 
  /**** INICIO DE LA SIMULACION ****/   
  int r = 0;

  while (tiempo <= 480){
    //Iterar sobre los servidores "direcciones[r]" pidiendo gasolina
    
    if(direcciones[r]==NULL){
      // Si llegamos al final de la lista, reiniciar.
      r = 0;
      usleep(100000);
    }

    if ((capMax-inventario)>=38000){

      // Verificar si el servidor r no respondió al pedir tiempos
      if (tiempos[r] == 500){ 
	r = r +1;
	continue;
      }

      // Pedir gasolina al servidor num r, almacenar respuesta en buffer gasolina
      char gasolina[20];
      char **result2 = pedir_gasolina_1( &nombre_pointer, clnts[r] );
      if ( result2 == (char **)NULL){
	clnt_perror( clnts[r], "Error al conectar con servidor");
	r= r+1;
	continue;
      }else{
	strcpy(gasolina,*result2);
      }
      
      // Procesar respuesta del servidor
      if (strcmp(gasolina,"noDisponible") == 0){

	fprintf(LOG,"Peticion: %d minutos, %s , No disponible, %d litros \n",
		tiempo, nombres[r],inventario);

	// Pedir gasolina al siguiente servidor en la lista
	r = r + 1; 
	continue;

	// si su ticket no esta vigente o no tiene ticket 
      } else if ( strcmp(gasolina,"noTicket") == 0 ){

	 int *retoInt = pedir_reto_1(NULL,clnts[r]);
	 //printf("El numero que recibí: %d \n",*retoInt);

	 // convertir el reto de int a string
	 char retoStr[10];
	 sprintf(&retoStr[0],"%d",*retoInt);

	 // Aplicar el algoritmo MD5
	 unsigned char respUChar[16];
	 MDString (&retoStr[0],&respUChar[0]);
	 //printf("El unsigned char se envió como: \n");
	 //MDPrint (respUChar);
	 //printf("\n");
	
	 // Convertir la respuesta a String
	 char respString[33];
	 int i;
	 for(i = 0; i < 16; ++i)
	   sprintf(&respString[i*2], "%02x", (unsigned int)respUChar[i]);

	 //printf("Y si lo imprimo como string es : \n");
	 //printf("%s \n",&respString[0]);

	 // Para que RPC nos permita pasarlo como argumento
	 char *respStr= (char*) &respString[0];
	 
	 int *resp = enviar_respuesta_1(&respStr, clnts[r]);

	 // FALTA VER SI LLEGA BIEN
	 //printf("respuesta en enviar %d",*resp);
	 // Y si es NULL ?
	 
	 if ( *resp == -1){
	   printf("error al autentificarse");

	   // PONER EN EL LOG AUTENTICACION FALLIDA
	   r = r + 1;
	   continue;
	 }else{
	   continue;
	   // para pedir gasolina de nuevo
	   // PONER EN EL LOG AUTENTICACION CORRECTA
	 } 
	 
      } else {
	// El ticket aun sigue vigente =) . Esperar y recibir gasolina.

	fprintf(LOG,"Peticion: %d minutos, %s, OK, %d litros  \n",
		tiempo, nombres[r],inventario);
	
	usleep((tiempos[r]+1)*100000); 
	
	pthread_mutex_lock(&mutex);
	inventario = inventario + 38000;
	pthread_mutex_unlock(&mutex);
	
	fprintf(LOG,"Llegada Gandola: %d minutos, %d litros \n", tiempo,inventario);
	if (inventario==capMax){ fprintf(LOG,"Tanque Full: %d minutos\n",tiempo);}
	
	// Reiniciar la busqueda de servidores activos
	r = 0;
      } 
      
    }// fin del if (que verifica si se necesita gasolina)

  }// Fin del while (Se acabó el tiempo)

  fclose(LOG);  
  return 0;
}
