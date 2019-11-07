//
//  confAction.c
//  sCrapper
//
//  Created by CELIA PIQUET on 17/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/scrapper.h"


void displayActionList(ListAction *actionList){
    int i;
    fprintf(stderr, "    Actions:{\n");
    for (i = 0; i < actionList->nbAction; i++){
        displayAction(actionList->actions[i]);
    }
    fprintf(stderr, "    }\n");
    
}

void cleanAction(Action *action){
    if (action->name == NULL){ // l'action a déjà été clean
        return;
    }
    int i;
    for (i = 0; i < action->sizeTypeList; i++){
        if (action->typeList != NULL && action->typeList[i] != NULL){
            free(action->typeList[i]);
            action->typeList[i] = NULL;
        }
    }
    if (action->typeList != NULL){
        free(action->typeList);
        action->typeList = NULL;
    }
    if (action->name != NULL){
        free(action->name);
        action->name = NULL;
    }
    if (action->url != NULL){
        free(action->url);
        action->url = NULL;
    }
}

void initAction(Action *action){
    int i;
    action->name = malloc(sizeof(char)*20);
    action->url = malloc(sizeof(char)*2000);
    action->maxDepth = 0;
    action->hasVersionning = 0;
    action->sizeTypeList = 10;
    action->typeList = malloc(sizeof(char)*action->sizeTypeList);
    action->nbType = 0;
    for (i = 0; i <  action->sizeTypeList; i++){
        action->typeList[i] = malloc(sizeof(char)*20);
    }
}

int completeAction(Action *action, FILE *fp, int *nbChar){
    int index = 0, actualAttributValueInt, i, sizeTabList = 0;
    char *actualLine = malloc(sizeof(char)*2500);
    char *actualAttributName = malloc(sizeof(char)*20);
    char *actualAttributValue = malloc(sizeof(char)*2000);
    char **actualAttributType = malloc(sizeof(char*)*10);
    
    if(actualAttributType != NULL && actualLine != NULL && actualAttributName != NULL && actualAttributValue != NULL){
        for (i = 0; i < 10; i++){
            actualAttributType[i] = malloc(sizeof(char)*20);
            if (actualAttributType[i] == NULL){
                return 0;
            }
        }
    } else {
        return 0;
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
            fprintf(stderr, "Attribut '%s' incorrect\n", actualAttributName);
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
    free(actualLine);
    free(actualAttributName);
    free(actualAttributValue);
    for (i = 0; i < 10; i++){
        if (actualAttributType[i] != NULL){
            free(actualAttributType[i]);
        }
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
        action->hasVersionning = attributValueInt%48;
    }
}

void setActionAttributType(Action *action, char **tabList, int sizeTabList){
    int i;
    for (i = 0; i < sizeTabList; i ++){
        strcpy(action->typeList[i], tabList[i]); 
        if (strlen(action->typeList[i]) < 20){
            action->typeList[i][strlen(action->typeList[i])] = '\0';
        }
    }
    action->nbType = sizeTabList;
}
void displayAction(Action action){
    int i;
    if(action.name == NULL){ // l'action est "clean"
        fprintf(stderr, "Cannot display the action because she was cleaned\n");
        return;
    }
    fprintf(stderr, "    {\n");
    fprintf(stderr, "        name : %s\n", action.name);
    fprintf(stderr, "        url : %s\n", action.url);
    fprintf(stderr, "        maxDepth : %d\n", action.maxDepth);
    fprintf(stderr, "        versioning : %d\n", action.hasVersionning);
    fprintf(stderr, "        type : \n");
    for (i = 0; i < action.nbType; i++){
        if (action.typeList != NULL && action.typeList[i] != NULL){
            fprintf(stderr, "            %s\n", action.typeList[i]);
        }
    }
    fprintf(stderr, "    }\n");
}
