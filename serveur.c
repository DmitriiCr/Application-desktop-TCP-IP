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
#include <stdbool.h>

#define PORT 5554

void handler(int num){
	wait(NULL);
}

void fonction_fils(int socketDeServer){
	//int buff;
	int commande;
	//int reponse;
	bool clientOn = true ;
	
	/*//printf("la commande envoye :%d",nombre);
	read(socketDeServer, &buff, sizeof(buff));	
	printf(" Le client à envoyé : %d \n",buff);
	int servInt = buff++; // modification que ferais le processus fils du serveur, ici +1 car modif arbitraire.
	printf("Nous envoyons au client : %d \n",buff);
	write(socketDeServer, &servInt, sizeof(servInt));
	printf("nous enverrons maintenant le menu au client");
	char menu[]="menu \n CHOIX DE LA COMMANDE\n 1 : BLA BLA BLA \n 2 : BLA BLA BLA\n" ;
	write(socketDeServer,&menu,sizeof(menu));*/
	
	
	/*read(socketDeServer, &commande, sizeof(commande));
	write(socketDeServer,&commande,sizeof(commande));*/
	//printf("la commande envoye :%d\n",commande);
	
	int nbFichier;
	char* Fichier;
	while(clientOn){
		printf("##################################\n##Attente de la commande\n##################################\n");
		read(socketDeServer, &commande, sizeof(commande));
		//write(socketDeServer,&commande,sizeof(commande));
		printf("La commande envoyé par le client est:%d\n",commande);
		switch(commande){
			case 0:
				//commande=0;
				printf("on effectue l'exit du client\n");
				write(socketDeServer,&commande,sizeof(commande));
				clientOn = false;
				break ;
			case 1 : 
				//commande = 1 ;
				printf("on effectue l'action 1\n");
				write(socketDeServer,&commande,sizeof(commande));
				break;
			case 2 :
				
				printf("on effectue l'action 2\n");
				write(socketDeServer,&commande,sizeof(commande));
				
//On récupère le nombre de valeur dans le tableau
				read(socketDeServer, &nbFichier , sizeof(nbFichier));
				char **listeFichier = (char**) malloc(nbFichier*sizeof(listeFichier));
				printf("\nLe nombre de fichier est :%d\n",nbFichier);

//On récupère les fichiers pour les mettrent dans une liste
				for (int j=0;j<nbFichier;j++){
					Fichier = (char*)malloc(100*sizeof(Fichier));
					read(socketDeServer, Fichier , sizeof(Fichier));
					listeFichier[j] = Fichier;
					
				}
				printf("\n La commande est toujours : %d\n",commande); 
				for (int k=0;k<nbFichier;k++){
					printf("L'élément %d est : %s\n",k,listeFichier[k]);
				}
				
				//appel à la vérif
				//renvoie de 2 listes: liste fichier valide et liste fichier non valide
				
				break ;
			default :
				printf("Mauvaise commande, veuillez envoyer une commande correcte\n");
				//commande = -1 ;
				write(socketDeServer,&commande,sizeof(commande));
		}
		//write(socketDeServer,&commande,sizeof(commande));
	}

	
	
	
}

void serveur(long port){
	
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
	listenSock.sin_port = htons(port);
	
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

	long *port = malloc(sizeof(long));
	
	if(argc != 2 ){
		printf("Attention il manque le numéro de port pour le serveur.\n ");
		printf("Usage ./serveur SERV_PORT. \n");
		exit(1);
	} else {
		*port = strtol(argv[1],NULL,10);
        if(*port<1 || *port>65535){
            printf("Attention votre numéro de port doit être compris entre 1 et 65535\n");
            printf("Usage ./server SERV_PORT\n");
            exit(1);
        }
        serveur(*port);
	}
	return 0;
}
