all:  funciones.o cliente.o servidor.o
	gcc funciones.o cliente.o -o bomba
	gcc funciones.o servidor.o -o centro
cliente.o: cliente.c funciones.h 
	gcc -c cliente.c -o cliente.o

servidor.o: servidor.c funciones.h
	gcc -c servidor.c -o servidor.o

funciones.o: funciones.c funciones.h
	gcc -c funciones.c -o funciones.o

clean:
	rm *.o bomba centro 
