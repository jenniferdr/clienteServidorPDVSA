#include "servicioPDVSA.h"
#include "tareasServidor.h"
#include <time.h>
#include <string.h>
#include <stdio.h>

/* Variables globales que se encuentran definidas
 * e inicializadas en "tareasServidor.h"
 */
extern int tiempo_actual;
extern int tiempo_respuesta;
extern int inventario;
extern pthread_mutex_t mutex;
extern FILE *LOG;
//extern struct ticket *tickets[MAX_SERVERS];
int numeroRn;


extern int ips[MAX_SERVERS];
extern int cuotas [MAX_SERVERS];
extern unsigned char *retos[MAX_SERVERS];

char **pedir_gasolina_1_svc(char ** bomba, struct svc_req *rqstp)
{ 
  int i =0 ;
  while (i < MAX_SERVERS && ips[i]!= -1){
    if (ips[i]==rqstp->rq_xprt->xp_raddr.sin_addr.s_addr){
      break;
    }
    i=i+1;
  }
   
  // VERIFICAR SI EL CLIENTE SE ENCONTRO DE VERDAD O NO ESTABA EN EL ARREGLO



  // 1. Buscar el ticket del cliente por su ip
  //    Si no existe retornarle al cliente el string "noTicket"
  //    si existe verificar si el tiempo es valido 
  //       Si el ticket no es valido ¿reportar en el LOG? y retornar "noTicket"
  //       Si es valido seguir ...
  static char *result;
  result= (char *) malloc(sizeof(char)*20);
  
  if (cuotas[i] < tiempo_actual){
    result = "noTicket";
    printf(" NO TENGO TICKETS \n"); 
  }
  else {
    printf("TENGO TICKETS \n");
    // Verificar si hay disponibilidad
    pthread_mutex_lock(&mutex);
    if( inventario >= 38000 ){
      inventario= inventario - 38000;
      if(inventario==0)fprintf(LOG,"Tanque vacío: %d minutos \n",tiempo_actual);
      strcpy(result,"enviando");
      fprintf(LOG,"Suministro: %d minutos, %s, OK, %d litros \n"
	      ,tiempo_actual,*bomba,inventario);
    }else{
      printf("Centro con inventario insuficiente \n");
      fprintf(LOG,"Suministro: %d minutos, %s, No disponible, %d litros \n"
	      ,tiempo_actual,*bomba,inventario);
      strcpy(result,"noDisponible");
    }
    pthread_mutex_unlock(&mutex);
  }

  return &(result);
}

int *pedir_tiempo_1_svc(void *argp, struct svc_req *rqstp)
{
  printf("client address: %u", rqstp->rq_xprt->xp_raddr.sin_addr.s_addr);
  
  int k = 0;
  while (k < MAX_SERVERS){
    // Y si ya esta lleno el arreglo de tickets ?
    if (ips[k]==-1){
       ips[k] = rqstp->rq_xprt->xp_raddr.sin_addr.s_addr; 
       break;
    }
    k=k+1;
  }
  //  printf("ip guardado %d \n", ips[k]);
  return &tiempo_respuesta;
}

int *pedir_reto_1_svc(void *argp, struct svc_req *rqstp)
{
  time_t reloj;
  time (&reloj);
  srand((unsigned int ) reloj);
  numeroRn = (int) reloj;

  printf("numero real %d \n",numeroRn);

  // convertir el numero a string
  char *numero = (char *) malloc(sizeof(char)*10);
  sprintf(numero,"%d",numeroRn);
  
  // Aplicar el hash al reto
  //unsigned char *resultado= (unsigned char *) malloc(sizeof(unsigned char)*16); 
  unsigned char resultado[16];
  MDString (numero,&resultado[0]);

  printf("Cuando genero el reto el mdprint es : \n");
  MDPrint (&resultado[0]);

  // convirtiendo a string
  char *md5string= (char *) malloc(sizeof(char)*33);
  int i;
  for(i = 0; i < 16; ++i)
    sprintf(&md5string[i*2], "%02x", (unsigned int)resultado[i]);

  printf("Y si lo imprimo como string es : \n");
  printf("%s \n",md5string);

  // Buscar el ticket del cliente por su ip (en el arreglo de tickets) 
  int k = 0;
  while (k < MAX_SERVERS){
    if (ips[k]==rqstp->rq_xprt->xp_raddr.sin_addr.s_addr ){
      retos[k]= md5string;
      //  MDPrint (retos[k]);
      //strcpy (retos[k],resultado); // encriptado
      break;
    }
    k=k+1;
  }
       
  return &numeroRn;
}

int *enviar_respuesta_1_svc(char ** resp, struct svc_req *rqstp)
{
  static int u = -1;
  int i = 0;
  int ip;
  //unsigned char *result = (unsigned char *) malloc(sizeof(unsigned char)*16);   
  //sprintf(result,"%s",*resp);
  //memcpy(result, *resp, 16);

  printf("El string se recibió como: \n");
  printf("%s",*resp);
  printf("\n"); 

  //*resp = "bla";
  //MDPrint (retos[0]);
  // int ver = ;
  //printf("funcion %d ", ver);
  //printf("\n");
  
  //printf("respuesta %s \n", *resp);

  //buscamos el ip del cliente
  while (i < MAX_SERVERS){
    if (ips[i]==rqstp->rq_xprt->xp_raddr.sin_addr.s_addr ){
      //MDPrint (retos[i]);
      printf("Y el string que tenia: \n");
      printf("%s\n",retos[i]);
      //printf("%s\n",result);
      if( strcmp (retos[i], *resp) == 0){
      //if ((compararUnsignedChar(result,retos[i]))==0){
	printf("COINCIDEN LAS CLAVES");
	cuotas[i]= tiempo_actual + 5 ;
	u =0;
	break;
      } else {
	printf("no coinciden claves");
	u = -1;
	break;
      }
    }
    i =i +1;
  }
  // VEr si de verdad esta enviendo -1
  return &(u);
}
