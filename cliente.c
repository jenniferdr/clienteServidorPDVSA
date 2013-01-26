#include <stdio.h>
#include <unistd.h>
#include <errno.h>

static void opciones(){


  printf("Sintaxis Correcta para el Servidor \n");
  printf("-n <Nombre De la bomba> \n");
  printf("-i <Numero del Inventario> \n");
  printf("-fc <Nombre del Archivo Central > \n");
  printf("-c <Numero de Consumo Promedio> \n");
  printf("-cp <Numero Capacida Maxima> \n");
}
	
int main(int argc, char **argv){
  char* nombre;
  int op;
  int inven  ;
  int consumo;
  int max = 10;

  if (argc !=7){
    perror( "Sintaxis Incorrecta");
    opciones();
   
  }
  else {
  opterr=0;
  // Obtener argumentos y validarlos  
  while((op=getopt(argc,argv,"n:i:c:"))!=-1)
    switch(op){
    case'n':
      nombre = optarg;
      printf("El nombre de la bomba es: %s \n", nombre);
      break;
    case'i':
      inven = atoi(optarg);
      
      if (0 < inven && inven < max){
	printf(" inventario es %d \n", inven);
      }
      else {
	perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");
      }
      break;
    case'c':

      consumo = atoi(optarg);
      if (0 <  consumo && consumo < 1000){
      printf("consumo es: %d \n",consumo );
      }
      else 
	perror("ERROR: El numero del consumo  \n Debe estar en [0-1000]");
      break;
   
    case '?':
      perror("ERROR :opcion desconocida\n");
      opciones();
      break;
   
    }
  }
}
 


