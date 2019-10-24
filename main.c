#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

FILE* getPage(char* savePath, char* url);

void getLinks(FILE *fp, char links[150][1000]);

void downloadType(char* link, int numDownload);

void downloadImage(char* link, int numDownload, char* typeImage);

int main() {

    char links[150][1000];

    //FILE* fp = getPage("/home/perniceni/Bureau/sCrapper/File/home.txt", "https://github.com/");
    
        ////FILE *fp = fopen("/home/ugo/Desktop/sCrapper/File/home.txt","r+");
    FILE *fp = fopen("/home/perniceni/Bureau/sCrapper/File/home.txt","r+");

    if(fp == NULL)
    	return 0;

    ////FILE *fp = fopen("/home/ugo/Desktop/sCrapper/File/home.txt","r+");
    //FILE *fp = fopen("/home/perniceni/Bureau/sCrapper/File/home.txt","r+");

 	getLinks(fp, links);

    for(int i = 0; i < 100; i++) // Affichage de la balise et de son contenu
	{
		printf("Lien n°%d : %s | taille string : %lu\n", i+1, links[i], strlen(links[i]));
		//downloadType(links[i], i);
	}

    //getTag("<a>", fp);

    //printf("%s", ReturnEndTag("<div>", 6));

    //printf("%d", tagSize("<script>"));

	//fclose(fp);

    return 0;
}

void getLinks(FILE *fp, char links[150][1000]){

	char* http = malloc(sizeof(char)*5);
	char* str = malloc(sizeof(char)*1000);
	char c;
	int occu = 0;
	int urlNumber = 0;

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

				for(int i = 0; i != occu - 2; i++) // Affichage de la balise et de son contenu
	    		{
	    			links[urlNumber][i] = str[i + 1];
	    		}


	    		urlNumber++;

	    	}

	    	c = fgetc(fp);
	    }

	}

	//free(http);

	//free(str);
}

void downloadType(char* link, int numDownload){

	if (strstr(link, ".png")!= NULL) {
		downloadImage(link, numDownload, "png");
	}
	else if(strstr(link, ".jpg")!= NULL){
		downloadImage(link, numDownload, "jpg");
	}
	else if(strstr(link, ".svg")!= NULL){
		downloadImage(link, numDownload, "svg");
	}

}

void downloadImage(char* link, int numDownload, char* typeImage){

	char* NomImage = malloc(sizeof(char) * 100);

	sprintf(NomImage, "/home/perniceni/Bureau/sCrapper/File/Images/Image%d.", numDownload);

	NomImage = strcat(NomImage, typeImage);

	FILE *fp = fopen(NomImage ,"wb");

	CURL *image;

    if (fp == NULL){
        printf("Erreur d'ouverture de fichier\n");
    }

	image = curl_easy_init();//Initialisation

    curl_easy_setopt(image, CURLOPT_URL, link);//Recuperation des informations au niv de l'URL
    curl_easy_setopt(image, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
    curl_easy_setopt(image, CURLOPT_FAILONERROR, 1L);//Gestion erreurs

    int result = curl_easy_perform(image);//Resultat telechargement

    if(result == CURLE_OK){
        printf("Téléchargement terminé !\n");
    } else {
        printf("ERROR: %s, link:-%s-\n", curl_easy_strerror(result), link);
    }

    fclose(fp);

    curl_easy_cleanup(image);//Vide les ressources de curl
}

FILE* getPage(char* savePath, char* url){

    CURL* curl;

    FILE* fp = fopen(savePath,"r+");

    int result;

    if (fp == NULL){
        printf("Erreur d'ouverture de fichier\n");
    }

    curl = curl_easy_init();//Initialisation
    curl_easy_setopt(curl, CURLOPT_URL, url);//Recuperation des informations au niv de l'URL
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs

    result = curl_easy_perform(curl);//Resultat telechargement

    if(result == CURLE_OK){
        printf("Téléchargement terminé !\n");
    } else {
        printf("ERROR: %s\n", curl_easy_strerror(result));
    }

    return fp;

    curl_easy_cleanup(curl);//Vide les ressources de curl

}