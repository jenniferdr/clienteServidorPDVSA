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

  int sock;
  struct sockaddr_in serv_addr;
  // printf("\n nombre%s",nombres[1]);
  /*Crear el socket */
  if((sock= socket(AF_INET,SOCK_STREAM,0))==-1)
    exit(0);
 
  struct hostent *he;
   /*Colocar aqui el nombre extraido del archivo!!!*/
  if( (he=gethostbyname(argv[1])) == NULL){
    /*Pedir gasolina a otro servidor*/
    herror("Error al identificar el host");
    exit(-1);
    /*Se pueden salvar valores para usar despues ¿?*/
  }
  
  /*Recopilar los datos del servidor en serv_addr*/
  serv_addr.sin_family = AF_INET;
  // FIX
  serv_addr.sin_port = htons(10323); 
  serv_addr.sin_addr = *((struct in_addr *)he->h_addr);  
  bzero(&(serv_addr.sin_zero),8);

  if(connect(sock,(struct sockaddr*)&serv_addr,sizeof(struct sockaddr_in))==-1){ 
    perror("connect() error\n");
    exit(-1);
  }
   

  int tiempoServi;;
  write(sock,"Tiempo",9);
  read(sock,&tiempoServi,sizeof(int));

  printf("Lei los datos %d \n",tiempoServi);
  return 0;


}
