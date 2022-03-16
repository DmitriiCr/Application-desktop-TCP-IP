#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netdb.h>
#include <netinet/in.h>
#include <signal.h>
#include <unistd.h>
#include <wait.h>

#define PORT 5554

void handler(int num){
	wait(NULL);
}

void fonction_fils(int socketDeServer){

	int buff;
	read(socketDeServer, &buff, sizeof(buff));
	
	printf(" Le client à envoyé : %d \n",buff);
	
	buff++; // modification que ferais le processus fils du serveur, ici +1 car modif arbitraire.
	
	printf("Nous envoyons au client : %d \n",buff);
	
	write(socketDeServer, &buff, sizeof(int));
}

void serveur(){
	
	signal(SIGCHLD, handler); // positionement du signal.
	
	int listenSocketFD, bindRes, len; // socket d'écoute, ... , ...
	
	struct sockaddr_in listenSock, cliSock; // diff ici cliSock est pointeur sur sockaddr
	
	
	listenSocketFD = socket(AF_INET, SOCK_STREAM, 0);
	if(listenSocketFD == -1){
		printf(" Socket creation failed mon ami.\n");
		exit(0);
	} else {
		printf("La socket à bien été crée !\n");
	}
	
	listenSock.sin_family = AF_INET;
	listenSock.sin_addr.s_addr = htonl(INADDR_ANY);
	listenSock.sin_port = htons(PORT);
	
	bindRes = bind(listenSocketFD,(struct sockaddr *)&listenSock,sizeof(struct sockaddr));
	
	if(bindRes == -1){
		printf("Socket binding failed mon ami.\n");
		exit(0);
	} else {
		printf("Socket binding was successfull.\n");
	}
	
	if((listen(listenSocketFD, 5))!= 0){
		printf("can't listen.\n");
		exit(0);
	} else {
		printf("Le serveur écoute.\n");
	}
	
	
	len = sizeof(cliSock);
	
	while(1){
		printf(" Nous sommes dans la boucle du serveur. ");
		int servSock = accept(listenSocketFD, (struct sockaddr *) &cliSock, (socklen_t *) &len);
		if(servSock == -1){
			printf("le serveur n'accepte pas la connexion.\n");
			exit(0);
		} else {
			printf("le serveur accepte la connexion.\n");
		}
		
		int forkRes = fork(); // à protéger
		if(forkRes < 0) {
            		perror("ERROR de fork");
            		exit(1);
        	}
		else if(forkRes == 0){
			// processus fils
			
			//on ferme la connection sur le socket d'écoute.
			close(listenSocketFD);
			
			fonction_fils(servSock);
			
			// on ferme la connection sur le socket de requetes.
			close(servSock);
			exit(0);
		} else {
			//processus père
			close(servSock);
		}
	}	
}

int main(int argc, char *argv[]){
	serveur();
	return 0;
}
