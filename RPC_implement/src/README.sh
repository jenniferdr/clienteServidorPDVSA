# Hacer rpcgen servicioPDVSA.x

# Instrucciones para agregar al _svc.c :
# #include "tareasServidor.h"
# antes de svc_run(); colocar: tareas_servidor(argc,argv);

# Compilar (Hacer Makefile)
gcc servicioPDVSA_server.c servicioPDVSA_svc.c funciones.c md5c.c tareasServidor.c -pthread -o centro

gcc cliente.c servicioPDVSA_clnt.c funciones.c md5c.c -pthread -o bomba

# Para escuchar por el puerto 111
#sudo rpcbind &

# CORRER SERVIDOR Y CLIENTE
# sudo ./centro -n C1 -cp 120000 -i 23 -t 50 -s 209 -p 10323
# ./bomba –n Bomba -cp 200000 –i 50000 –c 200 –fc dominios
