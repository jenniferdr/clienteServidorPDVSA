/* Servidor PDVSA  
 * Simulación de un centro de distribución gasolina que 
 * se encarga de atender peticiones de las bombas de gasolina 
 * en las cuales se hace un pedido de una gandola 
 * y asi surtir de gasolina a los clientes.
 *
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

#include "funciones.h"

int main(int argc, char *argv[]){

  char nombre[MAX_LONG];   // Nombre de este Centro
  int capMax;     // Capacidad Máxima (Litros)
  int inven;      // Inventario actual
  int tiempo;     // Tiempo de respuesta (minutos)
  int sumi;       // Suministro promedio (Litros*Minutos)
  int puerto;     
 
  obtener_argumentos_servidor(argc,argv,nombre,&inven, &tiempo,&sumi,&puerto,&capMax);

  int sock;
  obtener_socket_servidor(puerto,&sock);
  
  /*Datos para el socket cliente*/
  int sock2;
  struct sockaddr_in client_addr;
  int sizeSockadd = sizeof(struct sockaddr_in);

  //Configurar el log del servidor
  char nombre_log[MAX_LONG];
  sprintf(nombre_log,"log_%s.txt",nombre);
  FILE *log = fopen(nombre_log,"w");

  

  //Iniciar tiempo

  while(1){
    printf("Esperare que un cliente llegue \n");

    if((sock2=accept(sock,(struct sockaddr*)&client_addr,&sizeSockadd)) == -1){
      perror("Error al aceptar conexion con el cliente");
      continue;
    }

    printf("Llego el cliente, recibir solicitud \n");
    
    // Recibir solicitud
    char buff[9];
    int recibidos;
    if( (recibidos= recv(sock2,buff,9,0) < 9)){
      perror("Error al recibir el mensaje");
    }

    if(strcmp(buff,"Tiempo")==0){
      printf("Pidieron tiempo \n");
      write(sock2,&tiempo,sizeof(int));
    }else{
      // Crear el hijo 
    }
  }

  return 0;


}
