//
//  confListLinks.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

int addLinkToList(ListLinks *list, char *url){
    if (list == NULL){
        return 0;
    }
    if (isAlreadyInList(list, url)){
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
        if(strcmp(list->tabUrls[i],url) == 0){
            return 1;
        }
    }
    return 0;
}

void displayListLinks(ListLinks list){
    for (int i = 0; i < list.nbOfUrl; i++){
        printf("    %s\n", list.tabUrls[i]);
    }
}
