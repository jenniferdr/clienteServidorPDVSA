/* Servidor PDVSA  
 * Simulación de un centro de distribución gasolina que 
 * se encarga de atender peticiones de las bombas de gasolina 
 * en las cuales se hace un pedido de una gandola 
 * y asi surtir de gasolina a los clientes.
 *
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>
#include <strings.h>
#include "funciones.h"

#define SERVER_PORT 10323
#define BACK 5

int main(int argc, char *argv[]){

  /* Obtener args de la línea de comandos*/
  char* nombre;   // Nombre de este Centro
  int capMax;     // Capacidad Máxima (Litros)
  int inven;      // Inventario actual
  int tiempo;     // Tiempo de respuesta (minutos)
  int sumi;       // Suministro promedio (Litros*Minutos)
  int puerto;     // Puerto en el que escucha 
 
  argumentos_servidor(argc,argv,nombre,&inven, &tiempo,&sumi,&puerto,&capMax);

  int sock;
  struct sockaddr_in serv_addr;

  /*Crear el socket */
  if((sock= socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Error al inicializar el socket");
    exit(-1);
  }

  /*Asignar dirección en serv_addr / Hacer el bind*/
  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_port = htons(SERVER_PORT);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(serv_addr.sin_zero), 8);

  int sizeSockadd = sizeof(struct sockaddr_in);

  if(bind(sock,(struct sockaddr*) &serv_addr,sizeSockadd)==-1){
    perror("Error al hacer el bind del socket");
    exit(-1);
  }

  /*Establecer número máximo de clientes*/
  if(listen(sock, BACK) == -1){
    perror("Error al escuchar por el socket");
    exit(-1);    
  }

  /*Datos para el socket cliente*/
  int sock2;
  struct sockaddr_in client_addr;
  
  while(1){
    if((sock2=accept(sock,(struct sockaddr*)&client_addr,&sizeSockadd)) == -1){
      perror("Error al aceptar conexion con el cliente");
      continue;
    }
    
    
    
  }

  return 0;


}
