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
#include <netdb.h>
#include <strings.h>

/*Comentar lo que hace cada funcion*/

// Obtener argumentos..
extern void argumentos_cliente (int num, char ** arreglo, char *nombr
				,int* inve, int *consu, int* camax,  char* archi);


extern void argumentos_servidor (int num,char ** arreglo, char* nombr,int *inve,
				 int *tiem, int *sum,int *puert, int *camax);

// Imprimir por pantalla el uso correcto del comando
extern void opciones();


extern obtener_lista_dns(char *filename1, char ** nombre, char **direcciones, int *puertos );
