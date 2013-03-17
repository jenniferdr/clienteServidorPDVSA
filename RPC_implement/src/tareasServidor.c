
#include "tareasServidor.h"
#include <rpc/rpc.h>

// Structura para guardar los tickets
struct ticket {
  int ip;
  int cuota;
  char *reto;
};


int inventario;    // Inventario actual
int capMax;        // Capacidad Máxima (Litros)
int tiempo_actual; // minutos
int suministro;    // Suministro promedio (Litros*Minutos)
FILE *LOG;         // Archivo para la bitacora del servidor  
pthread_mutex_t mutex; // mutex sobre el inventario
int tiempo_respuesta; 
struct ticket tickets [MAX_SERVERS];


/* Hilo encargado de actualizar tiempo e inventario
 * Recibe un apuntador a una variable de tipo entero(tiempo)
 */
void *llevar_tiempo(void *arg_tiempo){
  
  int *tiempo= (int*) arg_tiempo;

  while(*tiempo<480){

    // Informar el tiempo cada 100 minutos
    if(*tiempo%100 == 0)printf("tiempo : %d \n",*tiempo);

    // Duerme un minuto y actualiza el tiempo 
    usleep(100000);
    *tiempo= *tiempo +1;
  
    // Actualizar inventario
    pthread_mutex_lock(&mutex);
    if(inventario+suministro<capMax){
      inventario= inventario + suministro;
    }else if(inventario!=capMax){
      inventario= capMax;
      fprintf(LOG,"Tanque full: %d minutos \n",tiempo_actual);
    }
    pthread_mutex_unlock(&mutex);

  }

  // Finalizar simulacion 
  usleep(30000);
  exit(0);
}

void tareas_servidor(int argc, char **argv){
  
  char nombre_centro[MAX_LONG];
  
  obtener_argumentos_servidor(argc,argv,nombre_centro,&inventario, 
			      &tiempo_respuesta,&suministro,&capMax);

  //Configurar el LOG del servidor
  char nombre_LOG[MAX_LONG];
  sprintf(nombre_LOG,"log_%s.txt",nombre_centro);
  LOG = fopen(nombre_LOG,"w");

  fprintf(LOG,"Inventario inicial: %d litros \n",inventario);
  if(inventario==0) fprintf(LOG,"Tanque vacio: 0 minutos \n");
  if(inventario==capMax) fprintf(LOG,"Tanque full: 0 minutos \n");
  fflush(LOG);

  // Inicializar 
  int k=0;
  while (k<MAX_SERVERS){
    tickets[k].cuota = -1;  
    tickets[k].ip= -1;
    
    k=k+1;

  }
  
  // Iniciar contador de tiempo 
  pthread_t contador_tiempo;
  tiempo_actual=0;
  pthread_create(&contador_tiempo,NULL,llevar_tiempo,&tiempo_actual);

}
