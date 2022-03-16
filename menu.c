#include <stdio.h>
#include <stdlib.h>
#include <string.h>
/*#include <sys/socket.h>
#include <netdb.h>
#include <unistd.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <time.h>
#include <stdbool.h>*/
#include <menu.h>

#define MAX 10000

#define PORT 5554


//////////////////////////////////
//Fonction Menu
//////////////////////////////////

int menu0(){
	int commandes;
	printf("AFFICHAGE DU MENU\n");
	printf("COMMANDE 0 : EXIT \n");
	printf("COMMANDE 1 : TOUT EST SOMBRE SAH\n");
	printf("COMMANDE 2 : LA VIE EST NWAR\n");
	printf("COMMANDE 3 :	\n");
	printf("donner une commande :");
	scanf("%d",&commandes);
	return commandes ;
}

//////////////////////////////////
//Fonction Sous- Menu
//////////////////////////////////
int menu1(){
	int commandes;
	printf("\nAFFICHAGE DU MENU DOWNLOAD\n");
	printf("COMMANDE 1 : AFFICHER DES FICHIERS\n");
	printf("COMMANDE 2 : DOWNLOAD DES FICHIERS\n");
	printf("Menu download à remplir\n");
	scanf("%d",&commandes);
	return commandes;
}

void menu2(){
	printf("AFFICHAGE DU MENU UPLOAD\n");
	printf("Menu upload à remplir \n");
}
