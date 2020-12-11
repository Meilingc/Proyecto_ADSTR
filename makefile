#######################
# Makefile libreria dinamica
#######################
# target
TARGET := main

# parametros
CC	:= gcc
sqlite	:=-lsqlite3
LIBS    :=-lemail -linforme
lib1	:=libinforme.so
lib2	:=libemail.so
INCLUDE := -I/home/pi/Desktop/Projecte-ADSTR/informe
info	:=--IP 172.20.0.21 --destinatari 1467071@campus.euss.org --remitent 1467071@campus.euss.org

#sudo cp libemail.so /usr/lib/
#link
#$(LIB):informe.o email.o
	#$(CC) -shared -fPIC -o $(lib1) informe.o
	#$(CC) -shared -fPIC -o $(lib2) email.o	
$(TARGET):main.c informe.o email.o
	#$(CC) -o informe  main.c -L. -linforme $(sqlite)
	$(CC) main.c -L. -linforme -lemail -o main $(sqlite)
	./informe $(info)

#compilar
informe.o:informe.c
	$(CC) -c informe.c $(sqlite)
email.o:email.c
	$(CC) -c email.c

# clean
clean:
	rm -fr *.o
