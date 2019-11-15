//
//  confParser.c
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

ListTask *readConf(char *confFilePath){
    FILE *fp = fopen(confFilePath, "r");
    if (fp == NULL){
        fprintf(stderr, "Erreur d'ouverture du fichier\n");
        return NULL;
    }
    
    // On récupère toutes les actions définies dans le fichier
    ListAction *allActions = getAllActionsFromConf(fp);
    if (allActions == NULL){
        fprintf(stderr, "Fichier de configuration corrompu (actions)\n");
        fclose(fp);
        return NULL;
    }
    
    
    // On récupère toutes les tâches définies dans le fichier
    ListTask *allTasks = getAllTasksFromConnf(fp, allActions);
    if (allTasks == NULL){
        fprintf(stderr,"Fichier de configuration corrompu (tasks)\n");
        cleanListAction(allActions);
        fclose(fp);
        return NULL;
    }
    
    fclose(fp);
    return allTasks;
}

int getOneLine(char* lineToUpdate, FILE *fp, int *charReaden, int sizeMaxOfLine){
    int sizeOfNewLine;
    if (fgets(lineToUpdate, sizeMaxOfLine, fp) == NULL){
        return 0;
    }
    sizeOfNewLine = (int)strlen(lineToUpdate);
    if (charReaden){
        *charReaden = *charReaden + sizeOfNewLine - 1;
    }
    if (sizeOfNewLine < sizeMaxOfLine){
        lineToUpdate[sizeOfNewLine-1] = '\0';
    }
    return 1;
}

void cleanOneLine(char *line){
    char firstChar = line[0];
    switch (firstChar) {
        case '=':
            cleanLineUntilSpecificChar(line, ' ', 0); //Au premier espace on devra stopper la chaine
            break;
        case '+':
            line[1] = '\0';
            break;
        case '{':
            cleanLineUntilSpecificChar(line, '}', 1);
            break;
        case '(':
            cleanLineUntilSpecificChar(line, ')', 1);
            break;
    }
}

void cleanLineUntilSpecificChar(char *line, char stop, int hasToStopNext){ // hasToStopNet nous permet de savoir si on doit supprimer le stop ou seulement le caractère d'après
    int i;
    int length = (int)strlen(line);
    for (i = 0; i < length-1; i++){
        if (line[i] == stop){
            line[i+hasToStopNext] = '\0';
            return;
        }
    }
}

int getAttributName(char *attributName, char *line){
    int indexOfLine = 1, indexOfAttributName = 0;
    while (indexOfLine < SIZE_MAX_OF_LINE_IN_CONF && line[indexOfLine] != ' ' && line[indexOfLine] != '>'){
        if (indexOfAttributName >= SIZE_MAX_STR_ATTRIBUT){
            return 0;
        }
        attributName[indexOfAttributName] = line[indexOfLine];
        indexOfLine ++;
        indexOfAttributName ++;
    }
    if(indexOfAttributName<SIZE_MAX_STR_ATTRIBUT){
        attributName[indexOfAttributName] = '\0';
    }
    return isCorrectAttributName(attributName);
}

// Récupère les attributs de types char*
void getAttributValueStr(char *actualLine, char *actualAttributValue, int index, int sizeActualAttributValue){
    int indexAttributValue = 0;
    index ++;
    while ((index < SIZE_MAX_OF_LINE_IN_CONF) && (actualLine[index] == '-' || actualLine[index] == '>')){
        index ++;
    }
    if (index < SIZE_MAX_OF_LINE_IN_CONF && actualLine[index] == ' '){
        index ++;
    }
    while (index < SIZE_MAX_OF_LINE_IN_CONF && actualLine[index] != '}' && indexAttributValue < sizeActualAttributValue){
        actualAttributValue[indexAttributValue] = actualLine[index];
        indexAttributValue ++;
        index ++;
    }
    if (indexAttributValue < sizeActualAttributValue){
        actualAttributValue[indexAttributValue] = '\0';
    }
}



// Récupère attributs de types char**
int getAttributValueTabStr(char *actualLine, char **attributValue, int index){
    index ++;
    int counterTabList = 0, counterInWord = 0;
    if (actualLine == NULL || attributValue == NULL){
        return 0;
    }
    while ((index < SIZE_MAX_OF_LINE_IN_CONF) && (actualLine[index] == '-' || actualLine[index] == '>')){
        index ++;
    }
    if (actualLine[index] == ' '){
        index ++;
    }
    if (actualLine[index] == '('){
        index ++;
    }
    while (index < SIZE_MAX_OF_LINE_IN_CONF && actualLine[index] != ')' && counterTabList < NB_MAX_OF_TYPES_PER_ACTION){ // Boucle pour tous les mots
        counterInWord = 0;
        while(index < SIZE_MAX_OF_LINE_IN_CONF && actualLine[index] != ',' && counterInWord < SIZE_MAX_STR_ATTRIBUT){ // Boucle pour un mot
            if (actualLine[index] == ')'){
                break;
            }
            attributValue[counterTabList][counterInWord] = actualLine[index];
            counterInWord ++;
            index ++;
        }
        if (counterInWord < SIZE_MAX_STR_ATTRIBUT){
            attributValue[counterTabList][counterInWord] = '\0';
        }
        if (index < SIZE_MAX_OF_LINE_IN_CONF && actualLine[index] == ','){
            index ++;
        }
        counterTabList ++;
    }
    return counterTabList;
}



int isCorrectAttributName(char *attributName){
    int isCorrect = 0;
    if (strcmp(attributName,"name") == 0){
        isCorrect = 1;
    } else if (strcmp(attributName,"url") == 0){
        isCorrect = 1;
    } else if (strcmp(attributName,"minute") == 0) {
        isCorrect = 1;
    } else if (strcmp(attributName,"second") == 0){
        isCorrect = 1;
    } else if (strcmp(attributName,"hour") == 0) {
        isCorrect = 1;
    } else if (strcmp(attributName,"max-depth") == 0){
        isCorrect = 1;
    } else if (strcmp(attributName,"versioning") == 0) {
        isCorrect = 1;
    } else if (strcmp(attributName,"type") == 0){
        isCorrect = 1;
    }
    return isCorrect;
}
