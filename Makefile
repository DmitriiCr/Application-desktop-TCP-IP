CC=clang -Wall

all : serveur client 

#Compilations des fichiers

%.o: %.c
	$(CC) -c $<
 
serveur.o: serveur.c  

client.o: client.c 


#Edition des liens



client: client.o
	$(CC) $^ -o $@

serveur: serveur.o
	$(CC) $^ -o $@
 
clean:
	rm -f client serveur *.o
