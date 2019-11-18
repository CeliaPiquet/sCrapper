//
//  curlFunctions.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

void getLinks(FILE *fp, Action *action){ // Récupère les liens de type "<a href:"http...">"
    char *cleanUrl = malloc(sizeof(char) * 1000);
    char *actualLine = malloc(sizeof(char) * SIZE_MAX_OF_LINE_DOWNLOADS);
    char *positionOfHref = NULL;
    int sizeOfUrl;
    actualLine[0] = '\0';

    if (fp && actualLine && cleanUrl){
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

int getHtmlPage(char* savePath, char* url){
    if (savePath && url){
        
        CURL* curl = NULL;

        FILE* fp = fopen(savePath,"wb+");

        int result;

        if (fp == NULL){
            printf("Error while opening the file\n");
            return 0;
        }
        
        curl = curl_easy_init();//Initialisation
        if (curl){
            curl_easy_setopt(curl, CURLOPT_URL, url);//Recuperation des informations au niv de l'URL
            curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            result = curl_easy_perform(curl);//Resultat telechargement
            
            
            if(result != CURLE_OK){
                fprintf(stderr, "    CURL ERROR: %s\n", curl_easy_strerror(result));
            }

            curl_easy_cleanup(curl);//Vide les ressources de curl
        }
        fclose(fp);
    }
    return 1;
}

void cleanCurlContentType(char *typeContentCurl){
    if (typeContentCurl){
        int index = 0, len = (int)strlen(typeContentCurl);
        while(index < len && typeContentCurl[index] != ';'){
            index ++;
        }
        typeContentCurl[index] = '\0';
    }
}


int checkContentType(CURL* curl, ListType listType){
    int result, i;
    char* typeContentCurl;
    
    if (listType.nbOfType == 0){    // Si aucun type n'est précisé on prend tout
        return 1;
    }
    if(curl) {
        if(curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &typeContentCurl) == 0){
            if (typeContentCurl){
                cleanCurlContentType(typeContentCurl);
                
                for (i = 0; i < listType.nbOfType; i ++){
                    result = strcmp(typeContentCurl, listType.tabType[i]);
                    
                    if(result == 0) {
                        return 1;
                    }
                }
            }
            
        }
        
    }
    return 0;
}
