
program SERVICIOPDVSA {

   version SERVICIOPDVSAVERS {
         
      string PEDIR_GASOLINA(string) = 1;

      int PEDIR_TIEMPO() = 2;

      int PEDIR_RETO() = 3;

      int ENVIAR_RESPUESTA(string) = 4;

   } = 1;

} = 0x20000001;