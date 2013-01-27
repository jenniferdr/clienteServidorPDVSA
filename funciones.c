#include "funciones.h"

void opciones(){


  printf("Sintaxis Correcta para el Servidor \n");
  printf("-n <Nombre De la bomba> \n");
  printf("-i <Numero del Inventario> \n");
  printf("-fc <Nombre del Archivo Central > \n");
  printf("-c <Numero de Consumo Promedio> \n");
  printf("-cp <Numero Capacida Maxima> \n");
}

void argumentos_cliente (int num,char ** arreglo, char* nombr, int *inve, int *consu, int *camax){
  int op;
  *camax = 10;
  if (num !=7){
    perror( "Sintaxis Incorrecta");
    opciones();
   
  }
  else {
    
      opterr=0;
      // Obtener argumentos y validarlos  
      while((op=getopt(num,arreglo,"n:i:c:"))!=-1)
	switch(op){
	case'n':
	  nombr = optarg;
	  printf("El nombre de la bomba es: %s \n", nombr);
	  break;
	case'i':
	  *inve = atoi(optarg);
	  
	  if (0 < *inve && *inve < *camax){
	    printf(" inventario es %d \n", *inve);
	  }
	  else {
	    perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");
	  }
	  break;
	case'c':
	  
	  *consu = atoi(optarg);
	  if (0 <  *consu && *consu < 1000){
	    printf("consumo es: %d \n",*consu );
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


void argumentos_servidor (int num,char ** arreglo, char* nombr, int *inve, int *tiem, int *sum,int *puert, int *camax){

  int op;
  bool cp = false;
  if (num !=13){
    perror( "Sintaxis Incorrecta");
    opciones();
    
  }
  else 
    {
      int i; 
      for(i = 0 ; i < 6 ; i++) 
	if(!strcmp(arreglo[i*2 + 1],"-cp")) { 
	       
	  *camax = atoi(arreglo[i*2+2]); 
	    
	  if (38000 < *camax && *camax < 380000){
	    cp = true; 
	    printf("La Capacidad maxima es: %d \n", *camax);
	  }
	  else { 
	    perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]");
	  }
	  break; 
	} 
	
	
      if(!cp){ 
	opciones(); 
	perror("ERROR : No coloco la opcion -cp  O ");
	perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]");
      } 
      else { 
	opterr=0;
	  
	while((op=getopt(num,arreglo,"n:i:t:s:p:c:"))!=-1)
	  switch(op){
	  case'n':
	    nombr = optarg;
	    printf("el nombre del centro es: %s \n", nombr);
	    break;
	  case'i':
	    *inve = atoi(optarg);
	      
	    if (0 < *inve && *inve < *camax){
	      printf(" inventario es %d \n", *inve);
	    }
	    else {
	      perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");
		
	    }
	    break;
	  case't':
    
	    *tiem = atoi(optarg);
	    if (0 <  *tiem && *tiem < 180){
	      printf("tiempo es: %d \n",*tiem );
	    }
	    else 
	      perror("ERROR: El numero del Tiempo no corresponde  \n Debe estar en [0 - 180]");
	    break;
	  case's':
	    *sum = atoi(optarg);
	    if (0 <  *sum && *sum < 10000){
	      printf("el suministro es: %d \n",*sum );
	    }
	    else 
	      perror("ERROR: , El numero del suministro no es valido \n DEbe estar en [0 - 10000]");
	    break;
	  case'p':
	    *puert = atoi(optarg);
		  
	    printf("mi puerto es: %d \n", *puert);
	    break;
	  case 'c':
	    break;
	  case '?':
	    perror("Error: Opcion Desconocida\n");
	    opciones();
	      
	    break;
	      
	  } 
      } 
    }

}
