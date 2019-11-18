//
//  confAction.c
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"
#include <curl/curl.h>

ListAction* getAllActionsFromConf(FILE *fp){
    int charReaden = 0;
    Action *actionToAdd = NULL;
    char *actualLine = malloc(sizeof(char)*SIZE_MAX_OF_LINE_IN_CONF);
    if (actualLine == NULL){
        return NULL;
    }
    
    ListAction *allActions = initListAction(NB_MAX_OF_ACTIONS_IN_CONF);
    if(allActions == NULL){
        free(actualLine);
        return NULL;
    }
    while (getOneLine(actualLine, fp, &charReaden, SIZE_MAX_OF_LINE_IN_CONF)){ // On lit chaque ligne
        cleanOneLine(actualLine);       // On nettoie la ligne des espaces superflues et des commentaires
        if(strcmp(actualLine,"=") == 0){
            actionToAdd = createOneActionFromConf(fp, &charReaden);
            if (actionToAdd == NULL){
                free(actualLine);
                return NULL;
            }
            if(!loadArgsFromAction(actionToAdd)){
                free(actualLine);
                return NULL;
            }
            if(!addActionToList(allActions, actionToAdd)){
                free(actualLine);
                return NULL;
            }
            fseek(fp, charReaden, SEEK_SET);
        }
    }
    free(actualLine);
    return allActions;
}

int addActionToList(ListAction *listAction, Action *actionToAdd){
    if (listAction->capacity == listAction->nbOfAction){
        return 0;
    }
    listAction->tabAction[listAction->nbOfAction] = *actionToAdd;
    listAction->nbOfAction ++;
    return 1;
}


Action* createOneActionFromConf(FILE *fp, int *charReaden){
    char *actualLine = malloc(sizeof(char)*SIZE_MAX_OF_LINE_IN_CONF);
    if (actualLine == NULL){
        return NULL;
    }
    Action *newAction = initAction();
    if(newAction == NULL){
        free(actualLine);
        return NULL;
    }
    while(getOneLine(actualLine, fp, charReaden, SIZE_MAX_OF_LINE_IN_CONF) && actualLine[0] != '='){
        cleanOneLine(actualLine);
        if (strlen(actualLine) == 0 || actualLine[0] == '+'){ // On ignore ces lignes
            continue;
        }
        if(!completeActionAttribut(newAction,actualLine)){ // L'attribut ou sa valeur ne sont pas bons, fichier corrompu
            free(actualLine);
            return NULL;
        }
    }
    free(actualLine);
    if(!completeListLinks(newAction)){
        return NULL;
    }
    return newAction;
}

void cleanListAction(ListAction *allActions){
    if (allActions != NULL && allActions->tabAction != NULL){
        for (int i = 0; i < allActions->nbOfAction; i++){
            cleanAction(&(allActions->tabAction[i]));
        }
        free(allActions->tabAction);
    }
}


void cleanAction(Action *action){
    int i;
    if (action != NULL){
        free(action->name);
        free(action->url);
        for (i = 0; i < action->typesToTarget.capacity; i++){
            free(action->typesToTarget.tabType[i]);
        }
        free(action->typesToTarget.tabType);
        for (i = 0; i < action->allUrlsWithDepth.capacity; i++){
            free(action->allUrlsWithDepth.tabUrls[i]);
        }
        free(action->allUrlsWithDepth.tabUrls);
    }
}

void cleanActionAttributVariables(char **attributName, char **charAttribut, char ***stringTabAttribut, char **attributValueInt){
    if (*attributName != NULL){
        free(*attributName);
    }
    if(*charAttribut != NULL){
        free(*charAttribut);
    }
    if(*attributValueInt != NULL){
        free(*attributValueInt);
    }
    if (*stringTabAttribut != NULL){
        for (int i = 0; i < NB_MAX_OF_TYPES_PER_ACTION; i ++){
            if ((*stringTabAttribut)[i] != NULL){
                free((*stringTabAttribut)[i]);
            }
        }
        free(*stringTabAttribut);
    }
}

int initActionAttributVariables(char **attributName, char **charAttribut, char **attributValueInt, char ***stringTabAttribut){
    *attributName = malloc(sizeof(char)*SIZE_MAX_STR_ATTRIBUT);
    if (*attributName == NULL){
        return 0;
    }
    
    *charAttribut = malloc(sizeof(char)*SIZE_MAX_URL); //On prend le plus grand possible, soit l'url
    if(*charAttribut == NULL){
        free(*attributName);
        return 0;
    }
    
    *attributValueInt = malloc(sizeof(char)*2);
    if(*charAttribut == NULL){
        free(*attributName);
        free(*charAttribut);
        return 0;
    }
    
    *stringTabAttribut = malloc(sizeof(char*)*NB_MAX_OF_TYPES_PER_ACTION);
    if (*stringTabAttribut != NULL){
        for (int i = 0; i < NB_MAX_OF_TYPES_PER_ACTION; i ++){
            (*stringTabAttribut)[i] = malloc(sizeof(char)*SIZE_MAX_STR_ATTRIBUT);
            if ((*stringTabAttribut)[i] == NULL){
                cleanTabOfString(*stringTabAttribut, NB_MAX_OF_TYPES_PER_ACTION);
                free(*attributName);
                free(*attributValueInt);
                free(*stringTabAttribut);
                return 0;
            }
        }
    } else {
        free(*attributName);
        free(*attributValueInt);
        free(*stringTabAttribut);
        return 0;
    }
    
    return 1;
}

int completeActionAttribut(Action *action, char *actualLine){
    int nbOfType;
    char *attributName = NULL, *attributValueInt = NULL, *attributValueStr = NULL;
    char **attributValueTabStr = NULL ;
   
    if(!initActionAttributVariables(&attributName, &attributValueStr, &attributValueInt, &attributValueTabStr)){
        return 0;
    }
    if (actualLine[0] != '{' || actualLine[0] == ' ' || actualLine[strlen(actualLine)-1] != '}'){ // on veut que la ligne soit de la forme {nom_de_lattribut -> valeure_de_lattribut}
        cleanActionAttributVariables(&attributName, &attributValueStr, &attributValueTabStr, &attributValueInt);
        return 0;
    }
    if (!getAttributName(attributName, actualLine)){
        cleanActionAttributVariables(&attributName, &attributValueStr, &attributValueTabStr, &attributValueInt);
        return 0;
    }
    
    if (strcmp(attributName,"name") == 0 || strcmp(attributName,"url") == 0){                            //la valeur de l'attribue sera une chaine de caractères
        getAttributValueStr(actualLine, attributValueStr, (int)strlen(attributName)+1, SIZE_MAX_URL);
        setActionAttributStr(attributName, action, attributValueStr);
    } else if (strcmp(attributName, "max-depth") == 0 || strcmp(attributName,"versioning") == 0){       // la valeur sera un entier
        getAttributValueStr(actualLine, attributValueInt, (int)strlen(attributName)+1, 2);
        if(!setActionAttributInt(attributName, action, attributValueInt)){
            return 0;
        }
    } else {                                                                                            // un tableau de string
        nbOfType = getAttributValueTabStr(actualLine, attributValueTabStr, (int)strlen(attributName)+1);
        if (nbOfType && !setActionAttributTabStr(attributName, action, attributValueTabStr, nbOfType)){
            cleanActionAttributVariables(&attributName, &attributValueStr, &attributValueTabStr, &attributValueInt);
            return 0;
        }
    }
    cleanActionAttributVariables(&attributName, &attributValueStr, &attributValueTabStr, &attributValueInt);
    return 1;
}

int setActionAttributInt(char *attributName, Action *action, char *attributValueInt){
    if (!strIsInt(attributValueInt)){
        fprintf(stderr, "Corrupted file: %s must be an integer value (you put '%s...')\n", attributName, attributValueInt);
        return 0;
    }
    if (strcmp("max-depth", attributName) == 0){
        action->maxDepth = atoi(attributValueInt);
    } else if (strcmp("versioning", attributName) == 0){
        action->hasVersionning = atoi(attributValueInt);
    }
    if (action->maxDepth > 2 || action->maxDepth < 0){
        fprintf(stderr, "Corrupted file : max-depth must be an integer between 0 and 2\n");
        return 0;
    }
    if (action->hasVersionning != 0 && action->hasVersionning != 1){
        fprintf(stderr, "Corrupted file : versioning must be 1 or 0\n");
        return 0;
    }
    return 1;
}


void setActionAttributStr(char *attributName, Action *action, char *attributValue){
    int sizeAttribut = (int)strlen(attributValue);
    if (strcmp("name", attributName) == 0){
        strcpy(action->name, attributValue);
        if (sizeAttribut < SIZE_MAX_STR_ATTRIBUT){
            action->name[sizeAttribut] = '\0';
        }
    } else if (strcmp("url", attributName) == 0){
        strcpy(action->url, attributValue);
        if (sizeAttribut < SIZE_MAX_URL){
            action->url[sizeAttribut] = '\0';
        }
    }
}

int setActionAttributTabStr(char *attributName, Action *action, char **attributValueTabStr, int sizeOfTabStr){
    if (attributName == NULL || action == NULL || attributValueTabStr == NULL){
        return 0;
    }
    ListType *types = &action->typesToTarget;
    int i = 0, sizeTypeToCopy;
    while (i < sizeOfTabStr && i < types->capacity){
        strcpy(types->tabType[i], attributValueTabStr[i]);
        sizeTypeToCopy = (int)strlen(attributValueTabStr[i]);
        if ( sizeTypeToCopy < SIZE_MAX_STR_ATTRIBUT){
            types->tabType[i][sizeTypeToCopy] = '\0';
        }
        i ++;
        types->nbOfType ++;
    }
    if (i != sizeOfTabStr){
        return 0;
    }
    return 1;
}

void displayAction(Action action){
    if(action.name == NULL || action.url == NULL){ // l'action est "clean"
        return;
    }
    int i;
    fprintf(stderr, "    {\n");
    fprintf(stderr, "       name : %s\n", action.name);
    fprintf(stderr, "       url : %s\n", action.url);
    fprintf(stderr, "       maxDepth : %d\n", action.maxDepth);
    fprintf(stderr, "       versioning : %d\n", action.hasVersionning);
    fprintf(stderr, "       nbOfUrlsToParse : %d\n", action.allUrlsWithDepth.nbOfUrl);
    fprintf(stderr, "       type : \n");
    for (i = 0; i < action.typesToTarget.nbOfType; i++){
        fprintf(stderr, "           %s\n", action.typesToTarget.tabType[i]);
    }
    fprintf(stderr, "    }\n");
}

int getActionName(char *actualLine, char *actionName){
    int index = 0, lenActualLine, indexActionName = 0;
    actionName[0] = '\0';
    lenActualLine = (int)strlen(actualLine);
    while (index <  lenActualLine && actualLine[index] != ',' && actualLine[index] != ')' && indexActionName < SIZE_MAX_STR_ATTRIBUT){
        actionName[indexActionName] = actualLine[index];
        index ++;
        indexActionName ++;
    }
    if (indexActionName < SIZE_MAX_STR_ATTRIBUT){
        actionName[indexActionName] = '\0';
    }
    if (index == lenActualLine){
        return 0;
    }
    return 1;
}

Action *getActionFromList(char *actionName, ListAction *allActions){
    int i;
    for (i = 0; i < allActions->nbOfAction; i++){
        if (strcmp(actionName, allActions->tabAction[i].name) == 0){
            return &allActions->tabAction[i];
        }
    }
    return NULL;
}
