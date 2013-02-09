/* ***
 * Implementación de funciones auxiliares.
 */

#include "funciones.h"
 

/*
 * Muestra por pantalla la sintaxis correcta para 
 * llamar al programa cliente (Bomba).
 */
void opciones_cliente(){
  printf("Sintaxis Correcta para el Cliente \n");
  printf("-n <Nombre De la bomba> \n");
  printf("-i <Numero del Inventario> \n");
  printf("-fc <Nombre del Archivo Central > \n");
  printf("-c <Numero de Consumo Promedio> \n");
  printf("-cp <Numero Capacida Maxima> \n");
}

/*
 * Muestra por pantalla la sintaxis correcta para 
 * llamar al programa servidor (Centro).
 */
void opciones_servidor(){
  printf("Sintaxis Correcta para el Servidor \n");
  printf("-n <Nombre Del Centro> \n");
  printf("-i <Numero del Inventario> \n");
  printf("-s <Cantidad De Suministro > \n");
  printf("-t <Tiempo> \n");
  printf("-cp <Numero Capacida Maxima> \n");
  printf(" -p  <Puerto>");
}

/*
 * Crea un socket TCP, configura el bind() y listen()
 * Toma como argumento el puerto en el que se desea asociar el socket y
 * un apuntador a entero.
 * El descriptor del socket será guardado en la dirección de memoria
 * apuntada por la variable sock.
 */
void obtener_socket_servidor(int puerto,int *sock){
  
  /*Crear el socket */
  if((*sock= socket(AF_INET,SOCK_STREAM,0))==-1){
    perror("Error al inicializar el socket");
    exit(-1);
  }
  fcntl(*sock,F_SETFL, O_NONBLOCK);
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

/*  Realiza la validacion de los argumentos del cliente*/
/* Que esten completos y que la informacion sea correcta */
void argumentos_cliente (int numArg,char ** arreglo, char* nombr, int *inve, int *consu, int *camax, char* archi){
  int op;

  if (numArg !=11){
    perror( "Sintaxis Incorrecta \n");
    opciones_cliente();
    exit(-1);
    
  }else{
    int i;
    printf("\n");
      
    for(i = 0 ; i < 5 ; i++){

      if(!strcmp(arreglo[i*2 + 1],"-c")) { 

	*consu= atoi(arreglo[i*2+2]);
	if (0 <= *consu && *consu <= 1000){

	}else{ 
	  perror("El numero del consumo debe estar en [0 - 1000]\n");

	  exit(-1);
	}

      }else if(!strcmp(arreglo[i*2 + 1],"-n")) {

	strcpy(nombr, arreglo[i*2+2]);

    
      }else if(!strcmp(arreglo[i*2 + 1],"-i")) { 
	
	*inve= atoi(arreglo[i*2+2]); 
	if (0 <= *inve && *inve <= *camax){

	}else{
	  perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");

	  exit(-1);
	}
       
      }else if(!strcmp(arreglo[i*2 + 1],"-fc")) { 
	strcpy(archi,arreglo[i*2+2]);

       
      }else if(!strcmp(arreglo[i*2 + 1],"-cp")) { 

	*camax = atoi(arreglo[i*2+2]);

      }else{
	perror("Sintaxis invalida");
	opciones_cliente();
	exit(-1);
      } 
    }
    if (38000 <= *camax && *camax <= 380000){

    }else{ 
      perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]\n");
     
      exit(-1);
    }
    
  }
}
/* Validar y obtener los argumentos del servidor */

void obtener_argumentos_servidor (int num,char ** arreglo, char* nombr, int *inve, int *tiem, int *sum,int *puert, int *camax){

  int op;
  bool cp = false;
  if (num !=13){
    perror( "Sintaxis Incorrecta");
    opciones_servidor(); 
  }else{
    int i; 
    for(i = 0 ; i < 6 ; i++){ 
      if(!strcmp(arreglo[i*2 + 1],"-cp")) { 
	  
	*camax = atoi(arreglo[i*2+2]);
	    
	if (38000 <= *camax && *camax <= 380000){
	  cp = true; 

	}else { 
	  perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]");
	  exit(-1);
	}
	break; 
      } 
    }	
       
    if(!cp){ 
      opciones_servidor(); 
      perror("ERROR : No coloco la opcion -cp  ");
      perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]");
      exit(-1);

    }else{ 
      opterr = 0;
	  
      while((op=getopt(num,arreglo,"n:i:t:s:p:c:"))!=-1)
	switch(op){
	case'n':
	  strcpy(nombr,optarg);

	  break;
	case'i':
	  *inve = atoi(optarg); 
	   
	  if (0 <= *inve && *inve <= *camax){

	  }else{
	    perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");

	  }
	  break;
	case't':
	  *tiem = atoi(optarg); 
	  if (0 <=  *tiem && *tiem <= 180){
	    //  printf("tiempo es: %d \n",*tiem );
	  }else{ 
	    perror("ERROR: El numero del Tiempo no corresponde  \n Debe estar en [0 - 180]");
	    exit(-1);  
	  }
	  break;
	case's':
	  *sum = atoi(optarg); 
	  if (0 <=  *sum && *sum <= 10000){

	  }else{ 
	    perror("ERROR: , El numero del suministro no es valido \n DEbe estar en [0 - 10000]");
	    exit(-1);
	   
	  }
	  break;
	case'p':
	  *puert = atoi(optarg); 
	  // Validar que el puerto este en un rango valido 

	  break;
	case 'c':
	  break;
	case '?':
	  perror("Error: Opcion Desconocida\n");
	  opciones_servidor();
	 
	  break;
	} 
    } 
  }
  
}

/* Lee del archivo DNS la informacion correspondiente*/
/* en los arreglos repectivos*/
int obtener_lista_dns(char *filename,char ** nombre, 
		      char **direcciones, int *puertos ){
   
  FILE *file = fopen ( filename, "r" ); 
  char *t;
  t = (char *) malloc (sizeof(char) * 128);
  while(t == 0){
    printf("Memoria no reservada, esperare un segundo");
    sleep(1);
    t = (char *) malloc (sizeof(char) * 128);
   
  }
    
  int i = 0;


  if ( file != NULL ){
    
    char linea [ MAX_LONG*2 ]; 
    while ( fgets ( linea, sizeof linea, file ) != NULL ){
      // Ignorar lineas en blanco
      if(strcmp(linea,"\n")==0)continue;

      t=strtok(linea,"&\n");
    
      int j=0;

      while(t!=NULL){
	if (j==0){
	  
	  nombre[i]= (char*) malloc(sizeof(char)*128); 
	  while(nombre[i] == 0){
	    printf("Memoria no reservada, esperare un segundo");
	    sleep(1);
	    nombre[i]= (char*) malloc(sizeof(char)*128); 
	  }
	  
	  strcpy(nombre[i],t);
	}
	
	if(j==1){
	  
	  direcciones[i]= (char*) malloc(sizeof(char)*128);
	  while(direcciones[i] == 0){
	    printf("Memoria no reservada, esperare un segundo");
	    sleep(1);
	    direcciones[i]= (char*) malloc(sizeof(char)*128); 
	    
	  }
	  strcpy(direcciones[i],t);
	  
	}
	if (j==2){
	  
	  puertos[i]=atoi(t); 
	}
	j=j+1;
	t = strtok (NULL, "&\n");
      }
      i=i+1;
      
    }
    
    free(t);
    puertos[i]=0;
    direcciones[i]= NULL;
    nombre[i]=NULL;
    fclose ( file );
    
}else{
    perror ( filename );
    exit(-1);
  }
  return 0;
}
/* Funcion Auxiliar que intercambia dos string*/
void swapLetras(char** a , char** b){
  
  char * aux;
  aux = *a;
  *a = *b;
  *b = aux;
}
/* funcion auxiliar que intercambia dos enteros*/
void swap(int *a , int *b){
  
  int aux;
  aux = *a;
  *a = *b;
  *b = aux;
}

