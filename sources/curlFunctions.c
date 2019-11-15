//
//  curlFunctions.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

void getLinks(FILE *fp, Action *action){
    char *cleanUrl = malloc(sizeof(char) * 1000);
    char *actualLine = malloc(sizeof(char) * SIZE_MAX_OF_LINE_DOWNLOADS);
    char *positionOfHref = NULL;
    int sizeOfUrl;
    actualLine[0] = '\0';

    if (fp != NULL && actualLine != NULL && cleanUrl != NULL){
        while(getOneLine(actualLine, fp, NULL, SIZE_MAX_OF_LINE_DOWNLOADS)){                    // On récupère chaque ligne du fichier
            positionOfHref = NULL;
            cleanUrl[0] = '\0';
            sizeOfUrl = 0;
            positionOfHref = strstr(actualLine,"href=\"https");     // On regarde si elle a un https ...
            if (positionOfHref == NULL){
                positionOfHref = strstr(actualLine,"href=\"http");  // ... ou un http
            }
            if (positionOfHref != NULL){                            // Si la ligne possède un lien qui commence bien par http ou https alors on le prend
                positionOfHref = positionOfHref + 6;                //Début du lien en "http..."

                while (*positionOfHref != 34 && *positionOfHref != 39){
                    sizeOfUrl ++;
                    positionOfHref = positionOfHref + 1;
                }
                strncpy(cleanUrl, positionOfHref-sizeOfUrl, sizeOfUrl);
                cleanUrl[sizeOfUrl] = '\0';

                if(!addLinkToList(&action->allUrlsWithDepth, cleanUrl)){
                    return;
                }
            }
            actualLine[0] = '\0';
        }
    }
    if (cleanUrl != NULL){
        free(cleanUrl);
    }
    if (actualLine != NULL){
        free(actualLine);
    }

}

void getHtmlPage(char* savePath, char* url){

    CURL* curl;

    FILE* fp = fopen(savePath,"wb+");

    int result;

    if (fp == NULL){
        printf("Erreur d'ouverture de fichier\n");
    }

    curl = curl_easy_init();//Initialisation
    curl_easy_setopt(curl, CURLOPT_URL, url);//Recuperation des informations au niv de l'URL
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    result = curl_easy_perform(curl);//Resultat telechargement

    if(result != CURLE_OK){
        printf("ERROR: %s\n", curl_easy_strerror(result));
    }

    curl_easy_cleanup(curl);//Vide les ressources de curl

    fclose(fp);
}


