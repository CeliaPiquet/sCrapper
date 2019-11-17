//
//  confListLinks.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

int completeListLinks(Action *action){
    fprintf(stderr, "Get all links of action '%s'...\n", action->name);
    int size, i, j, startList = 0;
    FILE *fp = NULL;
    char *filePath = malloc(sizeof(char)*200);
    if (filePath == NULL){
        return 0;
    }
    filePath[0] = '\0';
    
    if (!action){
        free(filePath);
        return 0;
    }
    
    if (action->url){
        int sizeOfUrl = (int)strlen(action->url);
        strcpy(action->allUrlsWithDepth.tabUrls[0], action->url);
        if (sizeOfUrl < SIZE_MAX_URL){
            action->allUrlsWithDepth.tabUrls[0][sizeOfUrl] = '\0';
        }
        action->allUrlsWithDepth.nbOfUrl ++;
    }

    sprintf(filePath, "%sdownloads/tmpFile.html", PARENT_PATH);
    filePath[strlen(PARENT_PATH)+strlen("downloads/tmpFile.html")] = '\0';
    
    for (i = 0; i < action->maxDepth; i++){
        fprintf(stderr, "   Depth %d running...\n", i+1);
        size = action->allUrlsWithDepth.nbOfUrl;
        for (j = startList; j < size; j++){
            if(action->allUrlsWithDepth.tabUrls[j] && filePath){
                if(getHtmlPage(filePath, action->allUrlsWithDepth.tabUrls[j])){
                    fp = fopen(filePath,"r");
                    if (fp != NULL){
                        getLinks(fp, action);
                        fclose(fp);
                        fp = NULL;
                    } else {
                        fprintf(stderr, "Erreur d'ouverture de fichier\n");
                    }
                    remove(filePath);
                }
            }
        }
        startList = size; // On ne regarde plus les url déjà vues
    }
    free(filePath);
    return 1;
}

int addLinkToList(ListLinks *list, char *url){
    if (list == NULL){
        return 0;
    }
    if (list && url && isAlreadyInList(list, url)){
        return 1;
    }
    if (list->capacity == list->nbOfUrl){
        ListLinks *newList = initListLinks(list->capacity*4);
        if (newList == NULL){
            return 0;
        }
        for (int i = 0; i < list->nbOfUrl; i++){
            strcpy(newList->tabUrls[i], list->tabUrls[i]);
            newList->tabUrls[i][strlen(list->tabUrls[i])] = '\0';
        }
        newList->nbOfUrl = list->nbOfUrl;
        *list = *newList;
        
    }
    strcpy(list->tabUrls[list->nbOfUrl], url);
    list->nbOfUrl ++;
    return 1;
}

int isAlreadyInList(ListLinks *list, char *url){
    int i;
    for (i = 0; i < list->nbOfUrl; i++){
        if (list->tabUrls[i] && url){
            if(strcmp(list->tabUrls[i],url) == 0){
                return 1;
            }
        }
    }
    return 0;
}

void displayListLinks(ListLinks list){
    for (int i = 0; i < list.nbOfUrl; i++){
        printf("    %s\n", list.tabUrls[i]);
    }
}
