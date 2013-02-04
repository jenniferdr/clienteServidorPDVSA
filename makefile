all:  src/funciones.o src/clientePrueba.o src/servidor.o
	gcc src/funciones.o src/clientePrueba.o -o bomba
	gcc src/funciones.o src/servidor.o -pthread -o centro
cliente.o: src/clientePrueba.c src/funciones.h 
	gcc -c src/clientePrueba.c -o src/clientePrueba.o

servidor.o: src/servidor.c src/funciones.h
	gcc -c src/servidor.c -o src/servidor.o

funciones.o: src/funciones.c src/funciones.h
	gcc -c src/funciones.c -o src/funciones.o

clean:
	rm src/*.o bomba centro 
