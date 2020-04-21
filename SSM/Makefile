all: server.o semantic.o
	gcc -L ../libparser -L ../librequest-0.4 -o server server.o semantic.o -lparser -lrequest 

server.o: server.c
	gcc -Wall -I ../libparser -I ../librequest-0.4/api -o server.o -c server.c 

semantic.o: semantic.c semantic.h
	gcc -Wall -I ../libparser -I ../librequest-0.4/api -o semantic.o -c semantic.c

clean:
	rm -f *.o server