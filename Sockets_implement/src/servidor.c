/* Servidor PDVSA  
 * Simulación de un centro de distribución gasolina que 
 * se encarga de atender peticiones de las bombas de gasolina 
 * las cuales hacen un pedido de una gandola de 38.000
 * litros de gasolina.
 *
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

#include "funciones.h"

int inventario;    // Inventario actual
int capMax;        // Capacidad Máxima (Litros)
int tiempo_actual; // minutos
int suministro;    // Suministro promedio (Litros*Minutos)
FILE *LOG;         // Archivo para la bitacora del servidor  
pthread_mutex_t mutex; // mutex sobre el inventario
int tiempo_respuesta; 

/* Hilo encargado de actualizar tiempo e inventario
 * Recibe un apuntador a una variable entera(tiempo)
 */
void *llevar_tiempo(void *arg_tiempo){
  pthread_detach(pthread_self());

  int *tiempo= (int*) arg_tiempo;
  while(*tiempo<480){
    
    usleep(100000);
    *tiempo= *tiempo +1;

    pthread_mutex_lock(&mutex);
    if(inventario+suministro<capMax){
      inventario= inventario + suministro;
    }else if(inventario!=capMax){
      inventario= capMax;
      fprintf(LOG,"Tanque full: %d minutos \n",tiempo_actual);
    }
    pthread_mutex_unlock(&mutex);

  }
  pthread_exit(0);
}

/* Hilo encargado de despachar las gandolas
 * Recibe un apuntador al socket por el que debe responder
 */
void *atender_cliente(void *socket){
  pthread_detach(pthread_self());
  int *mi_socket;
  mi_socket= (int*)socket;
  
  // Recibir solicitud
  char buff[MAX_LONG];
  int recibidos;

  if( (recibidos= recv(*mi_socket,buff,MAX_LONG,0)== -1)){
    perror("Error al recibir el mensaje\n");
    close(*mi_socket);
    *mi_socket=-1;
    pthread_exit(0);
  }

  if(strcmp(buff,"Tiempo")==0){
    //Pidieron tiempo
    write(*mi_socket,&tiempo_respuesta,sizeof(int));  
  }else{
    // Verificar si hay disponibilidad
    pthread_mutex_lock(&mutex);
    if( inventario >= 38000 ){
      inventario= inventario - 38000;
      if(inventario==0)fprintf(LOG,"Tanque vacío: %d minutos \n",tiempo_actual);
      write(*mi_socket,"enviando",sizeof(char)*9);
      fprintf(LOG,"Suministro: %d minutos, %s, OK, %d litros \n"
	      ,tiempo_actual,buff,inventario);
    }else{
      write(*mi_socket,"noDisponible",sizeof(char)*14);
      printf("Centro con inventario insuficiente \n");
      fprintf(LOG,"Suministro: %d minutos, %s, No disponible, %d litros \n"
	      ,tiempo_actual,buff,inventario);
    }
    pthread_mutex_unlock(&mutex);
  }

  // cerrar socket
  close(*mi_socket);
  // Liberar espacio del socket
  *mi_socket=-1;
  pthread_exit(0);

}

int main(int argc, char *argv[]){

  char nombre_centro[MAX_LONG];      
  int puerto;     
  int sockets[MAX_CONCURR];
 
  obtener_argumentos_servidor(argc,argv,nombre_centro,&inventario, 
			      &tiempo_respuesta,&suministro,&puerto,&capMax);

  //Configurar el LOG del servidor
  char nombre_LOG[MAX_LONG];
  sprintf(nombre_LOG,"log_%s.txt",nombre_centro);
  LOG = fopen(nombre_LOG,"w");

  fprintf(LOG,"Inventario inicial: %d litros \n",inventario);
  if(inventario==0) fprintf(LOG,"Tanque vacio: 0 minutos \n");
  if(inventario==capMax) fprintf(LOG,"Tanque full: 0 minutos \n");
  fflush(LOG);

  // Iicializar arreglo de sockets 
  int j;
  for(j=0; j<MAX_CONCURR;j++){
    sockets[j]=-1;
  }

  int sock;
  obtener_socket_servidor(puerto,&sock);

  /*Datos para el socket cliente*/
  struct sockaddr_in client_addr;
  int sizeSockadd = sizeof(struct sockaddr_in);

  // Iniciar contador de tiempo 
  pthread_t contador_tiempo;
  tiempo_actual=0;
  pthread_create(&contador_tiempo,NULL,llevar_tiempo,&tiempo_actual);

  //Inicio de la simulacion...
 
  while(tiempo_actual<480){
   
    int sock2;
    if((sock2=accept(sock,(struct sockaddr*)&client_addr,&sizeSockadd)) == -1){
      if(errno==EAGAIN || errno==EWOULDBLOCK){
	usleep(1000);
	continue;
      }
      perror("Error al aceptar conexion con el cliente/n");
      continue;
    }
    
    // Buscar un espacio libre para el socket
    int i;
    for(i=0; i<MAX_CONCURR;i++){
      if(sockets[i]==-1)break;
    }
    if( i==MAX_CONCURR){
       write(sock2,"noDisponible",sizeof(char)*14);
       printf("Se alcanzó el maximo de concurrencia \n");
       close(sock2);
       continue;       
    }
    sockets[i]= sock2;

    pthread_t trabajador;
    pthread_create(&trabajador,NULL,atender_cliente,&sockets[i]);
   
    fflush(LOG);
  }

  usleep(100000);

  fclose(LOG);
  close(sock);
  return 0;


}
