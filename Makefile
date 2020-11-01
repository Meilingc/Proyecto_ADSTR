#AQUEST MAKEFILE COMPILA I CONSTRUEIX L'EXECUTABLE DE CORREU SMTP I GET HTTP
FITA2: PRINCIPAL.o http_get.o email.o
	gcc -o FITA2 PRINCIPAL.o email.o http_get.o
		
PRINCIPAL.o: PRINCIPAL.c funcions.h
	gcc -c PRINCIPAL.c
	
email.o: email.c funcions.h
	gcc -c email.c
	
http_get.o: http_get.c funcions.h
	gcc -c http_get.c

clean:
	rm -f FITA2 *.o
