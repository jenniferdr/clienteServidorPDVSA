/* Cliente PDVSA  
 * Simulación de una bomba de gasolina que se encarga
 * de surtir gasolina a sus clientes y cuando tiene 
 * espacio suficiente para llenar nuevamente su tanque 
 * hace una peticion al servidor para llenarlo.
 *
 * Autores: Juliana Leon 
 *          Jennifer Dos Reis
 */

#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <netdb.h>


int main(int argc, char *argv[]){

  /* Obtener args de la línea de comandos*/
  char* nombre;  // Nombre de esta Bomba
  int capMax;    // Capacidad Máxima (Litros)
  int inven;     // Inventario actual
  int consumo;   // Consumo promedio (Litros*Minutos)
  char * archivo;
    argumentos_cliente(argc,argv,nombre,&inven,&consumo,&capMax, archivo);

  int sock;
  struct sockaddr_in serv_addr;

  /*Crear el socket */
  if((sock= socket(AF_INET,SOCK_STREAM,0))==-1)
    exit(0);

  /*Recopilar los datos del servidor en serv_addr*/
  

  return 0;


}