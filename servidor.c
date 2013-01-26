#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
static void opciones(){


  printf("Sintaxis Correcta para el Servidor \n");
  printf("-n <Nombre Del Centro de Suministro> \n");
  printf("-t <Numero Tiempo De Respuesta> \n");
  printf("-s <Numero Suministro Promedio> \n");
  printf("-p <Puerto> \n");
  printf("-cp <Numero Capacida Maxima> \n");

int main(int argc, char **argv){
  char* nombre;
  int op;
  int inven  ;
  int tiempo, sumi, puerto;
  int max = 10;
  bool cp = false;
  if (argc !=11){
    perror( "Sintaxis Incorrecta");
    opciones();
    
  }
  else 
    {
      /* int i; */
      /* for(i = 0 ; i < 14 ; i++) */
      /* 	if(!strcmp(argv[i + 1],"-cp")) { */
      /* 	  cp = true; */
      /* 	  max = atoi(argv[i]); */
      /* 	  break; */
      /* 	} */
    
   
      /* if(!cp){ */
      /* 	opciones(); */
      /* 	perror("ERROR : No coloco la opcion -cp"); */
      /* } */
      /* else { */
      opterr=0;

      while((op=getopt(argc,argv,"n:i:t:s:p:"))!=-1)
	switch(op){
	case'n':
	  nombre = optarg;
	  printf("el nombre del centro es: %s \n", nombre);
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
	case't':
    
	  tiempo = atoi(optarg);
	  if (0 <  tiempo && tiempo < 180){
	    printf("tiempo es: %d \n",tiempo );
	  }
	  else 
	    perror("ERROR: El numero del Tiempo no corresponde  \n Debe estar en [0 - 180]");
	  break;
	case's':
	  sumi = atoi(optarg);
	  if (0 <  sumi && sumi < 10000){
	    printf("el suministro es: %d \n",sumi );
	  }
	  else 
	    perror("ERROR: , El numero del suministro no es valido \n DEbe estar en [0 - 10000]");
	  break;
	case'p':
	  puerto = atoi(optarg);
      
	  printf("mi puerto es: %d \n", puerto);
	  break;
      
	case '?':
	  perror("Error: Opcion Desconocida\n");
	  opciones();
      
	  break;
      
	} 
      //      } 
    }
}
