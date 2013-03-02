all:  funciones.o funcionesServer.o cliente.o servidor.o
        gcc funciones.o funcionesServer cliente.o -pthread -o bomba
        gcc funciones.o servidor.o -pthread -o centro

cliente.o: cliente.c funciones.h servicioPDVSA.h
        gcc -c cliente.c -o cliente.o

servidor.o: servicioPDVSA_svc.c servicioPDVSA.h
	gcc -c servicioPDVSA_svc.c -o servidor.o

funcionesClient.o: servicioPDVSA_clnt.c servicioPDVSA.h
	gcc -c servicioPDVSA_clnt.c -o funcionesClient.o

funcionesServer.o: servicioPDVSA_svc.c servicioPDVSA.h
	gcc -c servicioPDVSA_svc.c -o funcionesServer.o

funciones.o: funciones.c funciones.h
        gcc -c funciones.c -o funciones.o

clean:
        rm src/*.o bomba centro  log_*


gcc servicioPDVSA_server.c servicioPDVSA_svc.c -o centro

gcc cliente.c servicioPDVSA_clnt.c -o bomba
