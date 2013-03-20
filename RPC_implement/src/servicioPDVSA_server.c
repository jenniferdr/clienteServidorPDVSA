#include "servicioPDVSA.h"
#include "tareasServidor.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

/* Variables globales que se encuentran definidas
 * e inicializadas en "tareasServidor.h"
 */
extern int tiempo_actual;
extern int tiempo_respuesta;
extern int inventario;
extern pthread_mutex_t mutex;
extern FILE *LOG;
int numeroRn;


extern int ips[MAX_SERVERS];
extern int cuotas [MAX_SERVERS];
extern unsigned char *retos[MAX_SERVERS];
/* Funcion que procesa el pedido de gasolina y verifica si el tickets esta vigente*/

char **pedir_gasolina_1_svc(char ** bomba, struct svc_req *rqstp)
{ 
  
  static char *result;
  result= (char *) malloc(sizeof(char)*20);
   while(result == 0){
    printf("Memoria no reservada, esperare un segundo");
    sleep(1);
    result = (char *) malloc (sizeof(char) * 128);
   
  }
    
  int i = 0 ;
  /* Ubicar el cliente que realiza la peticion */
  while (i < MAX_SERVERS && ips[i]!= -1){
    if (ips[i]==rqstp->rq_xprt->xp_raddr.sin_addr.s_addr){
      break;
    }
    i=i+1;
  }
  /* Si el cliente no es reconocido responde que no lo puede
     atender */
  if (i == MAX_SERVERS){
   
    strcpy(result,"noDisponible");
   
    return &(result);
  } else {

    if (cuotas[i] < tiempo_actual){
      strcpy(result,"noTicket");
    }
    else {
    
      // Verificar si hay disponibilidad
      pthread_mutex_lock(&mutex);
      if( inventario >= 38000 ){
	inventario= inventario - 38000;
	if(inventario==0)fprintf(LOG,"Tanque vacío: %d minutos \n",tiempo_actual);
      strcpy(result,"enviando");
      fprintf(LOG,"Suministro: %d minutos, %s, OK, %d litros \n"
	      ,tiempo_actual,*bomba,inventario);
      }else{
	//printf("Centro con inventario insuficiente \n");
	fprintf(LOG,"Suministro: %d minutos, %s, No disponible, %d litros \n"
		,tiempo_actual,*bomba,inventario);
	strcpy(result,"noDisponible");
      }
      pthread_mutex_unlock(&mutex);
    }
    
    return &(result);
  }
}
/* Funcion para pedir el tiempo del servidor*/
int *pedir_tiempo_1_svc(void *argp, struct svc_req *rqstp)
{
 
  int k = 0;
  /* Busca la primera posicion libre del arreglo 
     para colocar el siguiente cliente */
  while (k < MAX_SERVERS){
 
    if (ips[k]==-1){
       ips[k] = rqstp->rq_xprt->xp_raddr.sin_addr.s_addr; 
       break;
    }
    k=k+1;
  }
  return &tiempo_respuesta;
}
/* Funcion que retorna el reto que debe encriptar el cliente*/
int *pedir_reto_1_svc(void *argp, struct svc_req *rqstp)
{
  time_t reloj;
  time (&reloj);
  srand((unsigned int ) reloj);
  numeroRn = (int) reloj;

  // convertir el numero a string
  char numeroStr[10];
  sprintf(&numeroStr[0],"%d",numeroRn);
  
  // Aplicar el hash al reto 
  unsigned char resultado[16];
  MDString (&numeroStr[0],&resultado[0]);

  // convirtiendo a string
  char *md5string= (char *) malloc(sizeof(char)*33);
  int i;
  for(i = 0; i < 16; ++i)
    sprintf(&md5string[i*2], "%02x", (unsigned int)resultado[i]);

  int k = 0;
  while (k < MAX_SERVERS){
    if (ips[k]==rqstp->rq_xprt->xp_raddr.sin_addr.s_addr ){
      retos[k]= md5string;
      break;
    }
    k=k+1;
  }
       
  return &numeroRn;
}
/* Funcion que compara la clave encriptada del servidor con la 
   del cliente y otorga o rechaza un ticket*/
int *enviar_respuesta_1_svc(char ** resp, struct svc_req *rqstp)
{
  static int u = -1;
  int i = 0;
  int ip;
 
  /* Buscamos el ip del cliente */
  while (i < MAX_SERVERS){
    if (ips[i]==rqstp->rq_xprt->xp_raddr.sin_addr.s_addr ){
      
      /* Comparar que la clave del cliente y servidor coinciden*/
      if( strcmp (retos[i], *resp) == 0){
	/* otorgamos el ticket */
	cuotas[i]= tiempo_actual + 5 ;
	fprintf(LOG,"Autenticado al tiempo : %d \n", tiempo_actual);
	u = 0;
	break;
      } else {
	u = -1;
	fprintf(LOG,"Autenticacion fallida al tiempo : %d \n", tiempo_actual);
	break;
      }
    }
    i = i +1;
  }
  return &(u);
}
