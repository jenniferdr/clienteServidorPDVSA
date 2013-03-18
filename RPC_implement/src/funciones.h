/* ***
 * Archivo de funciones auxiliares.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>
#include <sys/types.h> 
#include <sys/socket.h>
#include <stdlib.h>
#include <string.h>
#include <netdb.h>
#include <strings.h>
#include <pthread.h>
#include <fcntl.h>
#include <errno.h>
#define BACK 5
#define MAX_LONG 128
#define MAX_SERVERS 50
#define MAX_CONCURR 128


// Para MD5

#include <time.h>
#include "global.h"
#include "md5.h"


#define MD5_CTX MD5_CTX
#define MDInit MD5Init
#define MDUpdate MD5Update
#define MDFinal MD5Final



void MDString (char * string ,unsigned char * result);
void MDPrint (unsigned char digest[16]);

/*
 * Muestra por pantalla la sintaxis correcta para 
 * llamar al programa cliente (Bomba).
 */
void opciones_cliente();

/*
 * Muestra por pantalla la sintaxis correcta para 
 * llamar al programa servidor (Centro).
 */
void opciones_servidor();

/*
 * Crea un socket TCP no bloqueante, configura el bind() y listen()
 * Toma como argumento el puerto en el que se desea asociar el socket y
 * un apuntador a entero.
 * El descriptor del socket será guardado en la dirección de memoria
 * apuntada por la variable sock.
 */
void obtener_socket_servidor(int puerto,int *sock);

/*  Realiza la validacion de los argumentos del cliente*/
/* Que esten completos y que la informacion sea correcta */
void argumentos_cliente (int numArg,char ** arreglo, char* nombr, int *inve,
			 int *consu, int *camax, char* archi);

/* Validar y obtener los argumentos del servidor */
void obtener_argumentos_servidor (int num,char ** arreglo, char* nombr, int *inve,
				  int *tiem, int *sum, int *camax);

/* Lee del archivo DNS la informacion correspondiente*/
/* en los arreglos repectivos*/
int obtener_lista_dns(char *filename,char ** nombre, 
		      char **direcciones/*, int *puertos*/ );

/* Funcion Auxiliar que intercambia dos string*/
void swapLetras(char** a , char** b);

/* funcion auxiliar que intercambia dos enteros*/
void swap(int *a , int *b);


