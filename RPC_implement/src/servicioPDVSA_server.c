
#include "servicioPDVSA.h"

char **pedir_gasolina_1_svc(char ** bomba, struct svc_req *rqstp)
{
  static char *result= "enviando";
  
  //strcpy(result,"enviando");

  return &(result);
}

int *pedir_tiempo_1_svc(void *argp, struct svc_req *rqstp)
{
  static int  result= 20;

      
  return &result;
}
