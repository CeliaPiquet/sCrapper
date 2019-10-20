#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

int tagSize(char* tag);

char* ReturnEndTag(char* tag, int tagSizeResult);

void getTag(char* tag,FILE *f);

void getLinks(FILE *fp);

int main() {
    CURL *curl;

    FILE *fp = fopen("/home/ugo/Desktop/sCrapper/File/home.txt","r+");
    //FILE *fp = fopen("/home/perniceni/Bureau/sCrapper/File/home.txt","r+");
    int result;

    char** UrlArray;

    if (fp == NULL){
        printf("Erreur d'ouverture de fichier\n");
        return 1;
    }

    // curl = curl_easy_init();//Initialisation
    // curl_easy_setopt(curl, CURLOPT_URL, "https://github.com/");//Recuperation des informations au niv de l'URL
    // curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
    // curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs

    // result = curl_easy_perform(curl);//Resultat telechargement

    // if(result == CURLE_OK){
    //     printf("Téléchargement terminé !\n");
    // } else {
    //     printf("ERROR: %s\n", curl_easy_strerror(result));
    // }

    //getTag("<a>", fp);

    getLinks(fp);

    //printf("%s", ReturnEndTag("<div>", 6));

    //printf("%d", tagSize("<script>"));

    fclose(fp);

    curl_easy_cleanup(curl);//Vide les ressources de curl

    return 0;
}

char* ReturnEndTag(char* tag, int tagSizeResult){
	char* result = malloc(sizeof(char)*(tagSizeResult+3));

	result[0] = '<';
	result[1] = '/';

	for(int i = 1; i <= tagSizeResult; i++) // Affichage de la balise et de son contenu
	{
	    result[i+1] = tag[i];
	}

	result[tagSizeResult + 2] = '>';
	result[tagSizeResult + 3] = '\0';

	return result;
}

int tagSize(char* tag){
	int u = 0;

	if(tag[0] == '<')
	{
		while(tag[u+1] != '>')
			u++;

		return u;
	}

	return 0;
}

void getTag(char* tag, FILE *fp){ // Balise à passer en paramètre, nombre de lettre de la balise ( sans les <> ), Fichier ouvert

	char* str = malloc(sizeof(char)*2000);
	//char str[200];
	//char* chaine = malloc(sizeof(char)*200);
	char c;
	int tagSizeResult = tagSize(tag);
	int occu = 0;

    printf("->getTag param: %s\n", tag);

	if (fp != NULL) // On verifie que le fichier existe et est ouvert...
    {

	    c = fgetc(fp); 

	    while (c != EOF) // tant qu'on est pas à la fin du document...
	    { 

	    	if(tag[occu] == c) // 1ère occurence de la balise
	    	{
	    		str[occu] = c;

	    		occu++;

	    	}
	    	else
	    	{
	    		occu = 0;
	    	}


	    	if (occu == tagSizeResult + 1) // Si la balise est complète alors on recupere tout son contenue. +1 pour le <
	    	{
	    		while(c != '>') // Tant que la balise n'est pas fermée tout est récupéré -!- A VOIR SI '>' EST EN TEXTE -!-
	    		{
	    			c = fgetc(fp);
	    			str[occu] = c;
	    			occu++;
	    		}

				for(int i = 0; i != occu; i++) // Affichage de la balise et de son contenu
	    		{
	    			printf("%c", str[i]);
	    		}

	    		printf("%s\n", ReturnEndTag(tag, tagSizeResult)); // Balise fermante

	    	}

	        c = fgetc(fp); 

	    } 

    }
    else
    	printf("Fichier NULL");
} 

void getLinks(FILE *fp){

	char* http = malloc(sizeof(char)*5);
	char* str = malloc(sizeof(char)*1000);
	char c;
	int occu = 0;
	int urlNumber = 0;

	char urlArray[100][1000];

	http = "\"http";

    printf("->getLinks\n");

	if (fp != NULL) // On verifie que le fichier existe et est ouvert...
    {

	    c = fgetc(fp); 

	    while (c != EOF) // tant qu'on est pas à la fin du document...
	    {
	    	if(c == http[occu])
	    	{
	    		str[occu] = http[occu];
	    		occu++;
	    	}
	    	else{
	    		occu = 0;
	    	}

	    	if(occu == 4)
	    	{
	    		while(c != 34)
	    		{
	    			c = fgetc(fp);
					str[occu] = c;
	    			occu++;
	    		}

	    		//printf("Lien : ");

				for(int i = 0; i != occu; i++) // Affichage de la balise et de son contenu
	    		{
	    			//printf("%c", str[i]);
	    			urlArray[urlNumber][i] = str[i];
	    		}

	    		//printf(" Url n°%d\n", urlNumber+1);

	    		urlNumber++;

	    	}

	    	c = fgetc(fp);
	    }


		for(int i = 0; i != 100; i++) // Affichage de la balise et de son contenu
	    	{
	    		printf("Lien n°%d : %s \n",i ,urlArray[i]);
	    	}

	}


	//free(http);

	//free(str);
}
