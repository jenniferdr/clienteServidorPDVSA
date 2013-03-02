# Para generar los _svc.c _clnt.c y el .h que incluyen nuestros programas servidor y cliente
rpcgen servicioPDVSA.x

gcc servicioPDVSA_server.c servicioPDVSA_svc.c funciones.c -pthread -o centro

gcc cliente.c servicioPDVSA_clnt.c funciones.c -pthread -o bomba

# Para escuchar por el puerto 111
sudo rpcbind &

# Para correr el servidor falso 
sudo ./centro


