/* ***
 * Archivo de funciones auxiliares.
 */

#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>

/*Comentar lo que hace cada funcion*/

// Obtener argumentos..
extern void argumentos_cliente (int num, char ** arreglo, char *nombr
				,int* inve, int *consu, int* camax,  char* archi);


extern void argumentos_servidor (int num,char ** arreglo, char* nombr,int *inve,
				 int *tiem, int *sum,int *puert, int *camax);

// Imprimir por pantalla el uso correcto del comando
extern void opciones();
