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

int inventario;  // Inventario actual
int capMax;      // Capacidad Máxima (Litros)
int tiempo_actual; // minutos
int suministro; // Suministro promedio (Litros*Minutos)
FILE *log;

// Hilo encargado de actualizar tiempo e inventario
void *llevar_tiempo(void *arg_tiempo){

  int *tiempo= (int*) arg_tiempo;
  while(*tiempo<=479){
    usleep(100000);
    *tiempo= *tiempo +1;
    if(inventario+suministro<capMax){
      inventario= inventario + suministro;
    }else{
      inventario= capMax;
      fprintf(log,"Tanque full: %d minutos \n",tiempo_actual);
    }
  }
}

// Hilo encargado de despachar las gandolas
void *atender_cliente(void *socket){
 
  int *mi_socket;
  mi_socket= (int*)socket;
  
  printf("Mi socket es: %d \n",*mi_socket);

  // Verificar si hay disponibilidad
  // Usar mutex desde aqui
  if( inventario >= 38000 ){
    inventario= inventario - 38000;
    if(inventario==0)fprintf(log,"Tanque vacío: %d minutos",tiempo_actual);
    write(*mi_socket,"enviando",sizeof(char)*9);
  }else{
    write(*mi_socket,"noDisponible",sizeof(char)*14);
  }
  // Cerrar mutex

  printf("Mi socket sigue siendo: %d\n",*mi_socket);
  
  // Liberar espacio del socket
  *mi_socket=-1;
  //pthread_exit();

}

int main(int argc, char *argv[]){

  char nombre_centro[MAX_LONG];  
  int tiempo_respuesta;    
  int puerto;     
  int sockets[MAX_CONCURR];
 
  obtener_argumentos_servidor(argc,argv,nombre_centro,&inventario, 
			      &tiempo_respuesta,&suministro,&puerto,&capMax);

  //Configurar el log del servidor
  char nombre_log[MAX_LONG];
  sprintf(nombre_log,"log_%s.txt",nombre_centro);
  log = fopen(nombre_log,"w");

  fprintf(log,"Estado inicial: %d \n",inventario);
  if(inventario==0) fprintf(log,"Tanque vacio: 0 minutos \n");
  if(inventario==capMax) fprintf(log,"Tanque full: 0 minutos \n");
  fflush(log);

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
    printf("Esperare que un cliente llegue \n");
    printf("Tiempo: %d \n",tiempo_actual);

    int sock2;
    if((sock2=accept(sock,(struct sockaddr*)&client_addr,&sizeSockadd)) == -1){
      perror("Error al aceptar conexion con el cliente");
      continue;
    }
    
    // Recibir solicitud
    char buff[9];
    int recibidos;
    if( (recibidos= recv(sock2,buff,9,0) < 9)){
      perror("Error al recibir el mensaje");
      // No se si hay que avisar al hijo 
      continue;
    }

    if(strcmp(buff,"Tiempo")==0){
      printf("Pidieron tiempo \n");
      write(sock2,&tiempo_respuesta,sizeof(int));

      // Buscar un espacio libre para el socket
      int i;
      for(i=0; i<MAX_CONCURR;i++){
	if(sockets[i]==-1)break;
      }
      sockets[i]= sock2;
      pthread_t trabajador;
      pthread_create(&trabajador,NULL,atender_cliente,&sockets[i]);
     
    }else{
      //Aqui va lo del if pero mientras para probar esta asi
    }
    fflush(log);
  }

  // Recordar cerrar el archivo al terminar
  return 0;


}
