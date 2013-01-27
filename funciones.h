#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <stdbool.h>



extern void argumentos_cliente (int num, char ** arreglo, char *nombr, int* inve, int *consu, int* camax);


extern  void opciones();

extern void argumentos_servidor (int num,char ** arreglo, char* nombr, int *inve, int *tiem, int *sum,int *puert, int *camax);
