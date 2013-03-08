
#include "tareasServidor.h"

int inventario;    // Inventario actual
int capMax;        // Capacidad Máxima (Litros)
int tiempo_actual; // minutos
int suministro;    // Suministro promedio (Litros*Minutos)
FILE *LOG;         // Archivo para la bitacora del servidor  
pthread_mutex_t mutex; // mutex sobre el inventario
int tiempo_respuesta; 

/* Hilo encargado de actualizar tiempo e inventario
 * Recibe un apuntador a una variable entera(tiempo)
 */
void *llevar_tiempo(void *arg_tiempo){
  pthread_detach(pthread_self());

  int *tiempo= (int*) arg_tiempo;
  while(*tiempo<480){
    
    usleep(100000);
    *tiempo= *tiempo +1;

    pthread_mutex_lock(&mutex);
    if(inventario+suministro<capMax){
      inventario= inventario + suministro;
    }else if(inventario!=capMax){
      inventario= capMax;
      fprintf(LOG,"Tanque full: %d minutos \n",tiempo_actual);
    }
    pthread_mutex_unlock(&mutex);

  }
  pthread_exit(0);
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
  
  // Iniciar contador de tiempo 
  pthread_t contador_tiempo;
  tiempo_actual=0;
  pthread_create(&contador_tiempo,NULL,llevar_tiempo,&tiempo_actual);

}