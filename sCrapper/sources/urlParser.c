//
//  urlParser.c
//  sCrapper
//
//  Created by CELIA PIQUET on 10/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/urlParser.h"

int isAlreadyInList(ListLinks* list, char* newUrl){
    for (int i=0; i<list->size; i++){
        if (strcmp(list->listUrls[i], newUrl)==0){
            return 1;
        }
    }
    return 0;
}

void addLinkToList(ListLinks* list, char* href){
    if (isAlreadyInList(list, href)){
        return;
    }
    if(list->size == list->capacity){
       // updateListCapacity(list);
    }
    strcpy(list->listUrls[list->size], href);
    list->size ++;
}

void getLinks(FILE *fp, ListLinks* listOfLinks){
    char *cleanUrl = malloc(sizeof(char) * 1000);
    char *actualLine = malloc(sizeof(char) * 2000);
    char *positionOfHref;
    int sizeOfUrl;

    if (fp != NULL && actualLine != NULL && cleanUrl != NULL) {
        while(getOneLine(actualLine, fp)){                  // On récupère chaque ligne du fichier
            sizeOfUrl = 0;
            positionOfHref = strstr(actualLine,"href=\"https");    // On regarde si elle a un https ...
            if (positionOfHref == NULL){
                positionOfHref = strstr(actualLine,"href=\"http"); // ... ou un http
            }
            if (positionOfHref){                 // Si la ligne possède un lien qui commence bien par http ou https alors on le prend
                positionOfHref = positionOfHref + 6; //Début du lien
                while (*positionOfHref != 34 && *positionOfHref != 39){
                    cleanUrl[sizeOfUrl] = *positionOfHref;
                    sizeOfUrl ++;
                    positionOfHref = positionOfHref + 1;
                }
                cleanUrl[sizeOfUrl] = '\0';
                addLinkToList(listOfLinks, cleanUrl);
            }
        }
        free(cleanUrl);
        cleanUrl = NULL;
        free(actualLine);
        actualLine = NULL;
        fclose(fp);
    }
}

void getPage(char* savePath, char* url){
    CURL* curl;
    FILE* fp = fopen(savePath,"w");
    int result;
    printf("get page : %s\n", url);
    if (fp == NULL || savePath == NULL || url == NULL){
        fprintf(stderr, "Erreur mémoire\n");
    }

    curl = curl_easy_init();//Initialisation
    curl_easy_setopt(curl, CURLOPT_URL, url);//Recuperation des informations au niv de l'URL
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs
    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

    result = curl_easy_perform(curl);//Resultat telechargement

    if(result != CURLE_OK){
        fprintf(stderr, "ERROR: %s\n", curl_easy_strerror(result));
    }
    
    curl_easy_cleanup(curl); //Vide les ressources de curl
    fclose(fp);
}
