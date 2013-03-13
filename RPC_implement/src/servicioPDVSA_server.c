
#include "servicioPDVSA.h"
#include "tareasServidor.h"
#include <time.h>

/* Variables globales que se encuentran definidas
 * e inicializadas en "tareasServidor.h"
 */
extern int tiempo_actual;
extern int tiempo_respuesta;
extern int inventario;
extern pthread_mutex_t mutex;
extern FILE *LOG;
extern struct ticket tickets;
int numeroRn;
char **pedir_gasolina_1_svc(char ** bomba, struct svc_req *rqstp)
{ // MODIFICAR
  // Primero agregar un argumento para q el cliente de su ip

  // 1. Buscar el ticket del cliente por su ip
  //    Si no existe retornarle al cliente el string "noTicket"
  //    si existe verificar si el tiempo es valido 
  //       Si el ticket no es valido ¿reportar en el LOG? y retornar "noTicket"
  //       Si es valido seguir ...

  static char *result;
  result= (char *) malloc(sizeof(char)*20);
  
  // Verificar si hay disponibilidad
  pthread_mutex_lock(&mutex);
  if( inventario >= 38000 ){
    inventario= inventario - 38000;
    if(inventario==0)fprintf(LOG,"Tanque vacío: %d minutos \n",tiempo_actual);
    strcpy(result,"enviando");
    fprintf(LOG,"Suministro: %d minutos, %s, OK, %d litros \n"
	    ,tiempo_actual,*bomba,inventario);
  }else{
    printf("Centro con inventario insuficiente \n");
    fprintf(LOG,"Suministro: %d minutos, %s, No disponible, %d litros \n"
	    ,tiempo_actual,*bomba,inventario);
    strcpy(result,"noDisponible");
  }
  pthread_mutex_unlock(&mutex);

  return &(result);
}

int *pedir_tiempo_1_svc(void *argp, struct svc_req *rqstp)
{
  printf("client address: %u", rqstp->rq_xprt->xp_raddr.sin_addr.s_addr);
  int k=0;
  // while (tickets[k].ip ==-1){
    k=k+1;
    // }
  //  tickets[k].ip = rqstp->rq_xprt->xp_raddr.sin_addr.s_addr; 
  return &tiempo_respuesta;
}

int *pedir_reto_1_svc(void *argp, struct svc_req *rqstp)
{
  // El cliente deberia pasar su ip para poderlo identificar 

  time_t reloj;
  time (&reloj);
  srand((unsigned int ) reloj);
  // encriptar la variable reloj
  
  // Buscar el ticket del cliente por su ip (en el arreglo de tickets) 
  // alli guardar la respuesta del reto en ticket[bla].reto
 
  numeroRn = (int) reloj;
  printf("numero real %d \n",numeroRn);

  // convertir el numero a string
  char *numero = (char *) malloc(sizeof(char)*10);
  sprintf(numero,"%d",numeroRn);
  
  unsigned char *resultado= (unsigned char *) malloc(sizeof(unsigned char)*16);  
  // Esto encripta una cadena de caracteres 
  //MDString (numero,resultado);
  //MDPrint (resultado);
  
  return &numeroRn;
}

int *enviar_respuesta_1_svc(char ** resp, struct svc_req *rqstp)
{
  int i=0;
  int ip;
  printf("respuesta %s \n", *resp);
  // buscamos el ips del cliente
 
  // donde debemos llenar este arreglo de ips?? Jenni- inicializ en tareasSer..
  // Se agregan las ips cada vez q se crea el ticket en pedir_gasolina
  // while (ips[i]!=ip && i < 99){
  //i = i+1;
  //}
  // puede ser 99 o NULL creo mejor NULL o -1 pero hay que inicializar
  // Podemos usar la constante MAX_SERVERS
  if (i==99){
    printf("error el cliente no se escuentra");
    return (0);// retonnar codigo de error
  }
  // comparar la respuesta que nos dio el cliente 
  // if( retos[i]== *resp /* No comparar asi, con strcomp*/ ){
    // como la respuesta fue correcta otorgo un ticket
    //cuota[i]= tiempo_actual + 5 ;
   
  // printf("renovado el ticket del cliente %d",ips[i]);
    // } else {
    // enviar codigo entero de error pues no coninciden claves
    printf("no coinciden claves");
    return (0);// retornar codigo de error
    // }
 
  // Xq se retorna 0 en ambos casos ?
  return (0);
}
