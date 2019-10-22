//
//  confAction.c
//  sCrapper
//
//  Created by CELIA PIQUET on 17/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../Headers/scrapper.h"


void initAction(Action *action){
    int i;
    action->name = malloc(sizeof(char)*20);
    action->url = malloc(sizeof(char)*2000);
    action->maxDepth = 0;
    action->versionning = 0;
    action->listType = malloc(sizeof(char)*10);
    action->sizeTypeList = 0;
    for (i = 0; i < 10; i++){
        action->listType[i] = malloc(sizeof(char)*20);
    }
}

int completeAction(Action *action, FILE *fp, int *nbChar){
    int index = 0, actualAttributValueInt, i, sizeTabList = 0;
    char *actualLine = malloc(sizeof(char)*2500);
    char *actualAttributName = malloc(sizeof(char)*20);
    char *actualAttributValue = malloc(sizeof(char)*2000);
    char **actualAttributType = malloc(sizeof(char)*10);

    for (i = 0; i < 10; i++){
        actualAttributType[i] = malloc(sizeof(char)*20);
    }
    
    while (getOneLine(actualLine, fp) && actualLine[0] != '='){ // Tant que la définition de l'action n'est pas fini on la traite
       
        *nbChar = *nbChar + (int)strlen(actualLine);
        if (actualLine[0] == '#' || actualLine[0] == '+' || strlen(actualLine) == 0){
            continue; // On ignore
        }
        
        // On "nettoie" la ligne pour éliminer ce qui ne nous intéresse pas : espaces inutiles, commentaires ..
        cleanLine(actualLine);
        
        // On récupère le nom de l'attribut sur cette ligne
        getAttributName(actualLine, actualAttributName, &index);
        
        if (!isCorrectAttributName(actualAttributName)){
            printf("Attribut '%s' incorrect\n", actualAttributName);
            return 0;
        }
        
        if(!strcmp("max-depth", actualAttributName) || !strcmp("versioning", actualAttributName)){
            // Gestion d'entiers
            getAttributValueInt(actualLine, &actualAttributValueInt, index);
            setActionAttributInt(actualAttributName, action, actualAttributValueInt);
            
        } else if (!strcmp("type", actualAttributName)){
            // Gestion d'un tableau de chaines de caractères
            getAttributValueType(actualLine, actualAttributType, index, &sizeTabList);
            setActionAttributType(action, actualAttributType, sizeTabList);
            
        } else {
            // Gestion des chaines de caractères simples
            getAttributValue(actualLine, actualAttributValue, index);
            setActionAttribut(actualAttributName, action, actualAttributValue);
        }
        
        index = 0;
        
    }
   // displayAction(action);
    free(actualLine);
    free(actualAttributName);
    free(actualAttributValue);
    for (i = 0; i < 10; i++){
        free(actualAttributType[i]);
    }
    free(actualAttributType);
    return 1;
}

void setActionAttribut(char *attributName, Action *action, char *attributValue){
    if (strcmp("name", attributName) == 0){
        strcpy(action->name, attributValue);
    } else if (strcmp("url", attributName) == 0){
        strcpy(action->url, attributValue);
    }
}

void setActionAttributInt(char *attributName, Action *action, int attributValueInt){
    if (strcmp("max-depth", attributName) == 0){
        action->maxDepth = attributValueInt%48;
    } else if (strcmp("versioning", attributName) == 0){
        action->versionning = attributValueInt%48;
    }
}

void setActionAttributType(Action *action, char **tabList, int sizeTabList){
    int i;
    for (i = 0; i < sizeTabList; i ++){
        strcpy(action->listType[i], tabList[i]);
        if (strlen(action->listType[i]) < 20){
            action->listType[i][strlen(action->listType[i])] = '\0';
        }
    }
    action->sizeTypeList = sizeTabList;
}
void displayAction(Action *action){
    int i;
    printf("ACTION : {\n");
    printf("    name : %s\n", action->name);
    printf("    url : %s\n", action->url);
    printf("    maxDepth : %d\n", action->maxDepth);
    printf("    versioning : %d\n", action->versionning);
    printf("    type : \n");
    for (i = 0; i < action->sizeTypeList; i++){
        printf("        %s\n", action->listType[i]);
    }
    printf("}\n");
}
