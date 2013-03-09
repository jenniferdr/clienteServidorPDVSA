
#include "servicioPDVSA.h"
#include "tareasServidor.h"
#include <time.h>
extern int tiempo_actual;
extern int tiempo_respuesta;
extern int inventario;
extern pthread_mutex_t mutex;
extern FILE *LOG;
int numeroRn;
char *retos[100];
int ips[100];
int cuota[100];
/* NOTA: En estas funciones se usan variables globales.
 * Se encuentran definidas en "tareasServidor.h"
 */

char **pedir_gasolina_1_svc(char ** bomba, struct svc_req *rqstp)
{
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
   
  return &tiempo_respuesta;
}

int *pedir_reto_1_svc(void *argp, struct svc_req *rqstp)
{
 
  time_t reloj;
  time (&reloj);
  srand((unsigned int ) reloj);
  // encriptar la variable reloj 
  // guardar lo encriptado en una estructura global
  // numeroRn = 5;
  numeroRn = (int )reloj;
  printf("numero real %d \n",numeroRn);
  return &numeroRn;
}

int *enviar_respuesta_1_svc(char ** resp, struct svc_req *rqstp)
{
  int i=0;
  int ip;
  // buscamos el ips del cliente
  while (ips[i]!=ip && i < 99){
    i = i+1;
  }
  // puede ser 99 o NULL creo mejor NULL o -1 pero hay que inicializar
  if (i==99){
    printf("error el cliente no se escuentra");
    return (0);// retonnar codigo de error
  }
  // comparar la respuesta que nos dio el cliente 
  if( retos[i]== *resp){
    // como la respuesta fue correcta otorgo un ticket
    cuota[i]= tiempo_actual + 5 ;
   
    printf("renovado el ticket del cliente %d",ips[i]);
  } else {
    // enviar codigo entero de error pues no coninciden claves
    printf("no coinciden claves");
    return (0);// retornar codigo de error
  }
 
  return (0);
}
