all:  src/funciones.o src/funcionesServer.o src/funcionesClient.o src/md5.o src/cliente.o src/servidor.o src/tareasServidor.o
	gcc src/funciones.o src/funcionesClient.o src/md5.o  src/cliente.o -pthread -o bomba
	gcc src/funciones.o src/servidor.o src/md5.o src/funcionesServer.o src/tareasServidor.o -pthread -o centro

src/cliente.o: src/cliente.c src/funciones.h src/servicioPDVSA.h
	gcc -c src/cliente.c -o src/cliente.o

src/servidor.o: src/servicioPDVSA_svc.c src/servicioPDVSA.h
	gcc -c src/servicioPDVSA_svc.c -o src/servidor.o

src/funcionesClient.o: src/servicioPDVSA_clnt.c src/servicioPDVSA.h
	gcc -c src/servicioPDVSA_clnt.c -o src/funcionesClient.o

src/funcionesServer.o: src/servicioPDVSA_server.c src/servicioPDVSA.h
	gcc -c src/servicioPDVSA_server.c -o src/funcionesServer.o

src/tareasServidor.o: src/tareasServidor.c src/tareasServidor.h src/funciones.h
	gcc -c src/tareasServidor.c -o src/tareasServidor.o 

src/funciones.o: src/funciones.c src/md5.h src/global.h src/funciones.h
	gcc -c src/funciones.c -o src/funciones.o

src/md5.o: src/md5c.c src/md5.h src/global.h
	gcc -c src/md5c.c -o src/md5.o

clean:
	rm src/*.o bomba centro  log_*
