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

int inven; // Inventario actual

// Hilo encargado de llevar el tiempo del servidor
void *llevar_tiempo(void *arg_tiempo){

  int *tiempo= (int*) arg_tiempo;
  while(*tiempo<=479){
    usleep(100000);
    *tiempo= *tiempo +1;
  }
}

void *atender_cliente(void *socket){
 
  int *mi_socket;
  mi_socket= (int*)socket;
  
  printf("Mi socket es: %d \n",*mi_socket);

  // Verificar si hay disponibilidad
  // Usar mutex desde aqui
  if( inven >= 38000 ){
    inven= inven - 38000;
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

  char nombre[MAX_LONG];   // Nombre de este Centro
  int capMax;     // Capacidad Máxima (Litros)
  int tiempo;     // Tiempo de respuesta (minutos)
  int sumi;       // Suministro promedio (Litros*Minutos)
  int puerto;     
  int tiempo_actual;
  int sockets[MAX_CONCURR];
 
  obtener_argumentos_servidor(argc,argv,nombre,&inven, &tiempo,&sumi,&puerto,&capMax);

  int sock;
  obtener_socket_servidor(puerto,&sock);
  
  /*Datos para el socket cliente*/
  struct sockaddr_in client_addr;
  int sizeSockadd = sizeof(struct sockaddr_in);

  //Configurar el log del servidor
  char nombre_log[MAX_LONG];
  sprintf(nombre_log,"log_%s.txt",nombre);
  FILE *log = fopen(nombre_log,"w");

  fprintf(log,"Estado inicial: %d \n",inven);
  if(inven==0) fprintf(log,"Tanque vacio: 0 minutos \n");
  if(inven==capMax) fprintf(log,"Tanque full: 0 minutos \n");
  fflush(log);

  // Iniciar contador de tiempo 
  pthread_t contador_tiempo;
  pthread_create(&contador_tiempo,NULL,llevar_tiempo,& tiempo_actual);

  int ultimo_tiempo= 0;

  while(tiempo<480){
    printf("Esperare que un cliente llegue \n");
    printf("Tiempo: %d \n",tiempo_actual);

    int sock2;
    if((sock2=accept(sock,(struct sockaddr*)&client_addr,&sizeSockadd)) == -1){
      perror("Error al aceptar conexion con el cliente");
      continue;
    }
    inven= inven + (tiempo - ultimo_tiempo)*sumi;
    ultimo_tiempo= tiempo;

    printf("Llego el cliente, recibir solicitud \n");
    
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
      write(sock2,&tiempo,sizeof(int));

      // Buscar un espacio libre para el socket
      int i;
      for(i=0; i<MAX_CONCURR;i++){
	if(sockets[i]==-1)break;
      }

      pthread_t trabajador;
      pthread_create(&trabajador,NULL,atender_cliente,&sockets[i]);
     
    }else{
      
    }
  }

  // Recordar cerrar el archivo al terminar
  return 0;


}
