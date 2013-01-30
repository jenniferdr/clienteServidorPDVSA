/* ***
 * Implementación de funciones auxiliares.
 */

#include "funciones.h"


void opciones(){
  printf("Sintaxis Correcta para el Cliente \n");
  printf("-n <Nombre De la bomba> \n");
  printf("-i <Numero del Inventario> \n");
  printf("-fc <Nombre del Archivo Central > \n");
  printf("-c <Numero de Consumo Promedio> \n");
  printf("-cp <Numero Capacida Maxima> \n");
}

/*
 * Crea un socket TCP, configura el bind() y listen()
 * Toma como argumento el puerto en el que se desea asociar el socket y
 * un apuntador a entero en el que se almacenará el descriptor del socket
 */
void obtener_socket_servidor(int puerto,int *sock){

  /*Crear el socket */
  if((*sock= socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Error al inicializar el socket");
    exit(-1);
  }

  struct sockaddr_in serv_addr;

  /*Asignar dirección del servidor en serv_addr para hacer el bind() */
  serv_addr.sin_family = AF_INET; 
  serv_addr.sin_port = htons(puerto);
  serv_addr.sin_addr.s_addr = INADDR_ANY;
  bzero(&(serv_addr.sin_zero), 8);

  int sizeSockadd = sizeof(struct sockaddr_in);

  if(bind(*sock,(struct sockaddr*) &serv_addr,sizeSockadd)==-1){
    perror("Error al hacer el bind del socket");
    exit(-1);
  }

  /*Establecer número máximo de clientes*/
  if(listen(*sock, BACK) == -1){
    perror("Error al escuchar por el socket");
    exit(-1);    
  }

}

void argumentos_cliente (int num,char ** arreglo, char* nombr, int *inve, int *consu, int *camax, char* archi){
  int op;
  if (num !=11){
    perror( "Sintaxis Incorrecta");
    opciones();
   
  }
  else {
    int i;
    printf("\n");      
    for(i = 0 ; i < 5 ; i++){
     
      if(!strcmp(arreglo[i*2 + 1],"-cp")) { 
	*camax = atoi(arreglo[i*2+2]); 
	
	if (38000 < *camax && *camax < 380000){
	  printf("La Capacidad maxima es: %d \n", *camax);
	 
	}
	
	else { 
	  perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]");
	  break;
	}
	continue;
      }
    }
   
    for(i = 0 ; i < 5 ; i++){
     
      if(!strcmp(arreglo[i*2 + 1],"-c")) { 
	*consu= atoi(arreglo[i*2+2]);
	if (0 < *consu && *consu < 1000){
	  printf("El consumo es %d \n", *consu);
	}
	else { 
	  perror("El numero del consumo debe estar en [0 - 1000]");
	  break;
	}
	continue;
      }
      else if(!strcmp(arreglo[i*2 + 1],"-n")) { 
	nombr= arreglo[i*2+2];
	printf("El nombre de la bomba es %s \n", nombr);
	continue;
      }
      else if(!strcmp(arreglo[i*2 + 1],"-i")) { 
	*inve= atoi(arreglo[i*2+2]);
	if (0 < *inve && *inve < *camax){
	  printf(" inventario es %d \n", *inve);
	}
	else {
	  perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");
	}
	continue;
      }
      else if(!strcmp(arreglo[i*2 + 1],"-fc")) { 
	archi= arreglo[i*2+2];
	printf("El archivo Central es %s\n", archi);
	continue;
      }	    
     
      else {
	break;
      }
    } 
  }
}



void obtener_argumentos_servidor (int num,char ** arreglo, char* nombr, int *inve, int *tiem, int *sum,int *puert, int *camax){

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
int obtener_lista_dns(char filename[],char ** nombre, 
		      char **direcciones, int *puertos ){
   
  // static const char filename[]="hola";
  FILE *file = fopen ( filename, "r" ); // Validar esta llamada
  char* t;
  int i=0;
  printf("filename%s",filename);
  if ( file != NULL ){
    
    char linea [ 128 ]; 
    while ( fgets ( linea, sizeof linea, file ) != NULL ){
      *t = strtok(linea," &");
      printf("token %s",t);
      int j=0;
      while(j < 3){
	printf("* %s%d%d *\n",t,j,i);
	if (j==0)
	  nombre[i]=t;
	if(j==1)
	  direcciones[i]=t;
	if (j==2)
	  puertos[i]=atoi(t);
	j=j+1;
	


	*t = strtok (NULL, " &");
      }
      printf("%s & %s & %d \n",nombre[i],direcciones[i],puertos[i]);
      
      i=i+1;
      //fputs ( linea, stdout ); 
    }
    fclose ( file );
 
  }else{
    perror ( filename ); 
  }
  return 0;
}

