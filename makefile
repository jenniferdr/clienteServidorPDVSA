all:  src/funciones.o src/cliente.o src/servidor.o
	gcc src/funciones.o src/cliente.o -o bomba
	gcc src/funciones.o src/servidor.o -pthread -o centro
cliente.o: src/cliente.c src/funciones.h 
	gcc -c src/cliente.c -o src/cliente.o

servidor.o: src/servidor.c src/funciones.h
	gcc -c src/servidor.c -o src/servidor.o

funciones.o: src/funciones.c src/funciones.h
	gcc -c src/funciones.c -o src/funciones.o

clean:
	rm src/*.o bomba centro 
