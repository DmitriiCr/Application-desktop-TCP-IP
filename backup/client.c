#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>

#define MAX 10000

#define PORT 5554

void client(char * nomServ){	
	int mySock, connSock;
	struct sockaddr_in servSock, cliSock;
	
	
	mySock = socket(AF_INET, SOCK_STREAM, 0);
	if(mySock == -1){
		printf(" Socket creation failed mon ami.\n");
		exit(0);
	} else {
		printf("La socket à bien été crée !\n");
	}
	
	struct hostent * myServer;
	myServer = gethostbyname(nomServ);
	
    	if (myServer == NULL) {
        	fprintf(stderr,"Attention ! Il n'y a pas d'host avec ce nom !\n");
        	exit(0);
   	}
	
	servSock.sin_family = AF_INET;
	memmove(&servSock.sin_addr.s_addr,myServer->h_addr,sizeof(myServer->h_addr));
	servSock.sin_port = htons(PORT);
	
	connSock = connect(mySock,(struct sockaddr *)&servSock,sizeof(servSock));
	if(connSock!= 0){
		printf("Connection with the server failed.\n");
		exit(0);
	} else {
		printf("Connection with the server was successfull.\n");
	}
	
	srand( time( NULL ) );
	int nombre = rand() % MAX;
	
	printf("On envoie %d au serveur.\n",nombre);
	write(mySock, &nombre, sizeof(nombre));

	int nombreR;
	while(read(mySock, &nombreR, sizeof(nombreR))==-1);
	
	printf("On reçois %d du serveur.\n", nombreR);
	
	close(mySock);
		
}

int main(int argc, char *argv[]){
	if(argc != 2){
		printf("Attention il manque l'adresse IP du serveur.\n Usage ./client SERV_IP_ADDRESS. \n");
		exit(1);
	} else {
		client(argv[1]);
	}
	
	return 0;
}
