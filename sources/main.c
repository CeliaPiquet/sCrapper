#include "../headers/scrapper.h"

/*
	TO DO : 
	- algo recherche des URLS, gestion des URLS avec JS ? ex:ga.src = ('https:' == document.location.protocol ? 'https://ssl' : 'http://www') + '.google-analytics.com/ga.js';

	- allocation dynamique des données
	- libération de la mémoire

	- profondeur
*/

int main() {

	struct ListLinks* ListLinks = newEmptyListOfLinks();

	getPage("/home/ugo/Bureau/sCrapper/File/home.txt", "https://github.com/");

	FILE *fp = fopen("/home/ugo/Bureau/sCrapper/File/home.txt","r");

    if(fp == NULL){

    	printf("Erreur lors de l'ouverture du fichier.");
    	return 0;
    }

	getLinks(fp, ListLinks);

	ListOfLinksToStringDebug(ListLinks);

	fclose(fp);

    return 0;
}

void getLinks(FILE *fp, struct ListLinks* ListLinks){

	char* http = malloc(sizeof(char)*4);
	char* str = malloc(sizeof(char)*1000);
	char* tmpCleanUrl = malloc(sizeof(char) * 1000);

	char c;
	int occu = 0;

	http = "http";

    printf("->getLinks\n");

	if (fp != NULL) // On verifie que le fichier existe et est ouvert...
    {
	    c = fgetc(fp); 

	    while (c != EOF) // tant qu'on est pas à la fin du document...
	    {
	    	if(c == 34 || c == 39)
	    	{
				c = fgetc(fp);

				while(c == http[occu]){
					str[occu] = http[occu];

					c = fgetc(fp);
					occu++;
				}

				if(occu == 4){

		    		while(c != 34 && c != 39){

		    			c = fgetc(fp);
						str[occu] = c;
		    			occu++;
		    		}

		    		str[occu] = '\0';

		    		for(int i = 0; i != occu; i++){ // Affichage de la balise et de son contenu
		    			tmpCleanUrl[i] = str[i];
		    		}

		    		tmpCleanUrl[occu - 1] = '\0';

		    		//printf("tmpCleanUrl: %s\n", tmpCleanUrl);

		    		ListOfLinksAdd(ListLinks, tmpCleanUrl, 0);

				}

				else{
					occu = 0;
				}
	    	}

	    	c = fgetc(fp);

	    }

	}
}


// -- les fonctions ci-dessous sont à actualiser --


/*
void downloadType(char* link, int numDownload){

	if(isImage(link) != 0)
	{
		downloadDoc(link, numDownload, isImage(link), "Images");
	}
	else if(strstr(link, ".js")!= NULL)
	{
		downloadDoc(link, numDownload, ".js", "Js");
	}
	else if(strstr(link, ".css")!= NULL)
	{
		downloadDoc(link, numDownload, ".css", "Css");
	}
}

char* isImage(char* link){

	if (strstr(link, ".png")!= NULL){
		return ".png";
	}
	else if(strstr(link, ".jpg")!= NULL){
		return ".jpg";
	}
	else if(strstr(link, ".svg")!= NULL){
		return ".svg";
	}

	return 0;
}

void downloadDoc(char* link, int numDownload, char* extension, char* file){

	char* NameDoc = malloc(sizeof(char) * 100);

	sprintf(NameDoc, "/home/ugo/Bureau/sCrapper/File/%s/Link%d",file ,numDownload);
	//sprintf(NameDoc, "/home/perniceni/Bureau/sCrapper/File/Images/Link%d.", numDownload);

	NameDoc = strcat(NameDoc, extension);

	printf("--%s--", NameDoc);

	FILE *fp = fopen(NameDoc ,"wb");

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
        printf("Download completed !\n");
    } else {
        printf("ERROR: %s, link:-%s-\n", curl_easy_strerror(result), link);
    }

    fclose(fp);

    curl_easy_cleanup(image);//Vide les ressources de curl
}
*/
void getPage(char* savePath, char* url){

    CURL* curl;

    FILE* fp = fopen(savePath,"w");

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

    curl_easy_cleanup(curl);//Vide les ressources de curl

    fclose(fp);
} 