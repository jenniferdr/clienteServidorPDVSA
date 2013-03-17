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
  printf("-cp <Numero Capacidad Maxima> \n");
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
  printf("-cp <Numero Capacidad Maxima> \n");
  //printf(" -p  <Puerto>");
}


/*  Realiza la validacion de los argumentos del cliente*/
/* Que esten completos y que la informacion sea correcta */
void argumentos_cliente (int numArg,char ** arreglo, char* nombr, int *inve,
			 int *consu, int *camax, char* archi){
  int op;

  if (numArg !=11){
    printf( "Sintaxis Incorrecta \n");
    opciones_cliente();
    exit(-1);
    
  }else{
    int i;
    printf("\n");
      
    for(i = 0 ; i < 5 ; i++){

      if(!strcmp(arreglo[i*2 + 1],"-c")) { 

	*consu= atoi(arreglo[i*2+2]);
	if (0 > *consu || *consu > 1000){
  	  perror("El numero del consumo debe estar en [0 - 1000]\n");
	  exit(-1);

	}

      }else if(!strcmp(arreglo[i*2 + 1],"-n")) {

	strcpy(nombr, arreglo[i*2+2]);

    
      }else if(!strcmp(arreglo[i*2 + 1],"-i")) { 
	
	*inve= atoi(arreglo[i*2+2]); 
	if (0 > *inve || *inve > *camax){
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
    if (38000 > *camax || *camax > 380000){
      perror("El Numero de Capacidad Maxima \n Debe estar en [38000 - 3800000]\n"); 
      exit(-1);
    }  
  }
}

/* Validar y obtener los argumentos del servidor */
void obtener_argumentos_servidor (int num,char ** arreglo, char* nombr, int *inve,
				  int *tiem, int *sum, int *camax){

  int op;
  bool cp = false;
  if (num !=11){
    printf( "Sintaxis Incorrecta \n");
    opciones_servidor(); 
    exit(-1);
  }else{
    int i; 
    for(i = 0 ; i < 6 ; i++){ 
      if(!strcmp(arreglo[i*2 + 1],"-cp")) { 
	  
	*camax = atoi(arreglo[i*2+2]);
	    
	if (38000 <= *camax && *camax <= 3800000){
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
	  
      while((op=getopt(num,arreglo,"n:i:t:s:c:"))!=-1)
	switch(op){
	case'n':
	  strcpy(nombr,optarg);

	  break;
	case'i':
	  *inve = atoi(optarg); 
	  
	  if (0 > *inve || *inve > *camax){
	    perror("ERROR: El numero del Inventario no corresponde\n  Debe estar en [0 - capacidadMaxima]");
	    exit(-1);
	  }
	  break;
	case't':
	  *tiem = atoi(optarg); 
	  if (0 >  *tiem || *tiem > 180){
	    perror("ERROR: El numero del Tiempo no corresponde  \n Debe estar en [0 - 180]");
	    exit(-1);
	  }
	  break;
	case's':
	  *sum = atoi(optarg); 
	  if (0 >  *sum || *sum > 10000){
	     perror("ERROR: , El numero del suministro no es valido \n DEbe estar en [0 - 10000]");
	    exit(-1);
	  }
	  break;
	case 'c':
	  break;
	case '?':
	  perror("Error: Opcion Desconocida\n");
	  opciones_servidor();
	  exit(-1);
	  break;
	} 
    } 
  }
  
}

/* Lee del archivo DNS la informacion correspondiente*/
/* en los arreglos repectivos*/
int obtener_lista_dns(char *filename,char ** nombre, 
		      char **direcciones/*,int *puertos*/ ){
   
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
	    printf("Memoria no reservada, esperare un segundo\n");
	    usleep(100000);
	    nombre[i]= (char*) malloc(sizeof(char)*128); 
	  }
	  
	  strcpy(nombre[i],t);
	}
	
	if(j==1){
	  
	  direcciones[i]= (char*) malloc(sizeof(char)*128);
	  while(direcciones[i] == 0){
	    printf("Memoria no reservada, esperare un segundo\n");
	    usleep(100000);
	    direcciones[i]= (char*) malloc(sizeof(char)*128); 
	    
	  }
	  strcpy(direcciones[i],t);
	  
	}
	/*if (j==2){
	  
	  puertos[i]=atoi(t); 
	  }*/
	j=j+1;
	t = strtok (NULL, "&\n");
      }
      i=i+1;
      
    }
    
    free(t);
    //puertos[i]=0;
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

/* Copyright (C) 1990-2, RSA Data Security, Inc. Created 1990. All
rights reserved.

RSA Data Security, Inc. makes no representations concerning either
the merchantability of this software or the suitability of this
software for any particular purpose. It is provided "as is"
without express or implied warranty of any kind.

These notices must be retained in any copies of any part of this
documentation and/or software.
 */

/* Digests a string and prints the result.
 */
void MDString (char *string,unsigned char *digest)
{
  MD5_CTX context;
  //unsigned char digest[16];
  unsigned int len = strlen (string);

  MDInit (&context);
  MDUpdate (&context, string, len);
  MDFinal (digest, &context);

  //  printf ("MD5 (\"%s\") = ", string);
  //MDPrint (digest);
  //printf ("\n");
}

/* Prints a message digest in hexadecimal.
 */
void MDPrint (unsigned char digest[16])
{
  unsigned int i;

  for (i = 0; i < 16; i++)
    printf ("%02x", digest[i]);
}
