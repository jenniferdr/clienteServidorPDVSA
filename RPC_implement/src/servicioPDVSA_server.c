
#include "servicioPDVSA.h"
#include "tareasServidor.h"

extern int tiempo_actual;
extern int tiempo_respuesta;
extern int inventario;
extern pthread_mutex_t mutex;
extern FILE *LOG;

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
  return &tiempo_respuesta;
}

int *enviar_respuesta_1_svc(char ** resp, struct svc_req *rqstp)
{
  return &tiempo_respuesta;
}
