#include "site.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

//S'exécute avant que le serveur soit lancé
// Charge la configuration contenue dans le fichier sites.conf dans une liste chaînée
void loadMultisitesConf()
{
	multisitesConf = NULL;

	// On ouvre le fichier
	FILE* fichierConf = fopen("sites.conf", "r");
	if(fichierConf == NULL)
	//Lors d'une erreur, on ferme le serveur
	//Pas besoin d'envoyer un 500 (Internal Server Error) car aucune connexion n'est ouverte
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}

	// On récupère la première ligne
	char* ligne = malloc(500*sizeof(char));
	if(ligne == NULL)
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}

	Site *site;


	// On malloc() les deux champs code et fichier
	char* code = malloc(sizeof(char)*50);
	if(code == NULL)
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}
	char* codeFree = code;
	char* fichier = malloc(sizeof(char)*50);
	if(fichier == NULL)
	{
		perror("Erreur lors de l'initialisation");
		exit(-1);
	}


	// On parcourt le fichier
	while (fgets(ligne, 500, fichierConf) != NULL) {
		// Si on enregistre un nouveau site
		if (ligne[0] != '\t') {
			// On malloc l'espace
			site = malloc(sizeof(Site));
			if(site == NULL)
			{
				perror("Erreur lors de l'initialisation");
				exit(-1);
			}
			//
			char* e400 = calloc(sizeof(char),50);
			char* e404 = calloc(sizeof(char),50);
			char* e411 = calloc(sizeof(char),50);
			char* e418 = calloc(sizeof(char),50);
			char* e501 = calloc(sizeof(char),50);
			char* e505 = calloc(sizeof(char),50);
			site->e400 = e400;
			site->e404 = e404;
			site->e411 = e411;
			site->e418 = e418;
			site->e501 = e501;
			site->e505 = e505;

			// On malloc() les deux champs fqdn et dossier
			char* fqdn = malloc(sizeof(char)*50);
			if(fqdn == NULL)
			{
				perror("Erreur lors de l'initialisation");
				exit(-1);
			}
			char* dossier = malloc(sizeof(char)*50);
			if(dossier == NULL)
			{
				perror("Erreur lors de l'initialisation");
				exit(-1);
			}

			// On parse la ligne
			char *ptr = strtok(ligne, " ");
			strcpy(fqdn, ptr);
			ptr = strtok(NULL, " ");
			strcpy(dossier, ptr);

			// On retire les sauts à la ligne
			int c = 0;
			while (dossier[c] != '\0') {
				if ((dossier[c] == '\n') || (dossier[c] == '\r')) {
					dossier[c] = '\0';
				}
				c++;
			}

			// On enregistre les données parsées
			site->fqdn = fqdn;
			site->dossier = dossier;

			site->next = multisitesConf;
			multisitesConf = site;
		}
		// Si on enregistre un nouveau fichier d'erreur
		else {

			// On parse la ligne
			char *ptr = strtok(ligne, " ");
			strcpy(code, ptr);
			code++;
			ptr = strtok(NULL, " ");
			strcpy(fichier, ptr);

			// On retire les sauts à la ligne
			int c = 0;
			while (fichier[c] != '\0') {
				if ((fichier[c] == '\n') || (fichier[c] == '\r')) {
					fichier[c] = '\0';
				}
				c++;
			}

			// On enregistre les données parsées
			if      (!strcmp(code, "400")) strcpy(multisitesConf->e400, fichier);
			else if (!strcmp(code, "404")) strcpy(multisitesConf->e404, fichier);
			else if (!strcmp(code, "411")) strcpy(multisitesConf->e411, fichier);
			else if (!strcmp(code, "418")) strcpy(multisitesConf->e418, fichier);
			else if (!strcmp(code, "501")) strcpy(multisitesConf->e501, fichier);
			else if (!strcmp(code, "505")) strcpy(multisitesConf->e505, fichier);
		}
	}
	free(codeFree);
	free(fichier);
	free(ligne);
	fclose(fichierConf);
}

void unloadMultiSitesConf()
{
	Site* s = multisitesConf;
	Site* next;
	while(s != NULL)
	{
		free(s->e400);
		free(s->e404);
		free(s->e411);
		free(s->e418);
		free(s->e501);
		free(s->e505);

		free(s->fqdn);
		free(s->dossier);

		next = s->next;

		free(s);

		s = next;
	}
}