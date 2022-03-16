#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>
#include <menu.h>

#define MAX 10000
void nomsImages(int nbElem,char** listeElem){

	for (int i=0; i<nbElem; i++){
	
		listeElem[i]=(char*)malloc (100*sizeof(char*));
		scanf("%s",listeElem[i]);
		fflush(stdin);
	}
}




void client(char * nomServ, long port){	
	int mySock, connSock,commande ;
	struct sockaddr_in servSock;
	bool enMarche = true ;
	
	
	
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
	memcpy(&servSock.sin_addr.s_addr,myServer->h_addr,myServer->h_length);
	servSock.sin_port = htons(port);
	
	connSock = connect(mySock,(struct sockaddr *)&servSock,sizeof(servSock));
	if(connSock!= 0){
		printf("Connection au serveur échouée. Vérifiez le numéro de port.\n");
		exit(0);
	} else {
		printf("La connexion est établie.\n");
	}
	
	/*srand( time( NULL ) );
	int nombre = rand() % MAX;
	
	printf("On envoie %d au serveur.\n",nombre);
	write(mySock, &nombre, sizeof(nombre));*/

	//int nombreR;
	//char tab[10000];
	int reponse ;
	int sMenu;
	char** listeElem;
	//while(read(mySock, &nombreR, sizeof(nombreR))==-1);
	
	//printf("On reçois %d du serveur.\n", nombreR);
	
	while(enMarche){
		commande = menu0();
		write(mySock, &commande, sizeof(commande));
		read(mySock, &reponse, sizeof(reponse));
		printf("on a recu la réponse suivant : %d\n",reponse);
		switch(reponse){
			case 0 :
				printf("\nOn fait l'exit\n");
				
				enMarche = false ;
				break;
			case 1 :
				sMenu = menu1() ;
				printf("\nOn prépare le download\n");
				printf("Commande %d du download\n",sMenu);
				//printf("%s",tab);
				break;
			case 2 :
				menu2();
/*ON CREE LA LISTE DE NOM DE FICHIER A ENVOYER*/
				int nbElem;
				printf("\nOn prépare l'upload\n");
				printf("Nombre de fichiers upload ?");
				scanf("%d",&nbElem);
				listeElem = (char**) malloc(nbElem*sizeof(listeElem));
				nomsImages(nbElem,listeElem);
				/*for (int i= 0; i<nbElem;i++){
					printf("test7\n");
					printf("Elem %d est %s",i,listeElem[i]);
				}*/
/*ON ENVOIE LES FICHIERS DANS LE SERVEUR*/
//On envoie le nombre d'élement dans le serveur
				write(mySock,&nbElem,sizeof(nbElem));
				
//On envoie les fichier dans le serveur
				for (int j=0;j<nbElem;j++){
					write(mySock,listeElem[j],sizeof(listeElem));
				}
				free(listeElem);
				
				//printf("%s",tab)
				break;
			case -1 :
				printf("erreur veuillez entrer une commande parmi celle propose");
				break ;
		}		
		printf("\n\n");
	}
	printf("on sort bien du while, au revoir \n") ;
	
	
	
	close(mySock);
		
}

int main(int argc, char *argv[]){
	char *adresse = NULL;
	long *port = malloc(sizeof(long));
	if(argc != 3){
		if(argc==1){
			printf("Attention il manque l'adresse IP du serveur.\n Usage ./client SERV_IP_ADDRESS SERV_PORT. \n");
			exit(1);
		} else if(argc==2){
			printf("Attention il manque le numéro de port du serveur.\n Usage ./client SERV_IP_ADDRESS SERV_PORT. \n");
			exit(1);
		}
	} else {
		*port = strtol(argv[1],NULL,10);
		adresse = argv[2];
		if(*port<1 || *port>65535){
            printf("Attention votre numéro de port doit être compris entre 1 et 65535\n");
            printf("Usage ./client SERV_IP_ADDRESS SERV_PORT\n");
            exit(1);
        }
		client(adresse, *port);
	}
	return 0;
}
