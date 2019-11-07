//
//  confParser.c
//  sCrapper
//
//  Created by CELIA PIQUET on 15/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/scrapper.h"

// TODO :   vérifier les valeurs numériques entrées dans la conf avec signalement à l'utilisateur que le fichier n'est pas bon
//          paramétrer les Tasks
//          si possible faire une struct TabAction, qui aurait une taille variable
//          réfléchir si les Tasks sont toujours définies après les Actions
//          ATTENTION aux valeurs numériques qui peuvent être à 2 chiffres (gérer autrement que par des caractères)
//          gérer les espaces de fin de lignes


void cleanConf(ListTask *tasks, ListAction *actions){
    int i;
    for (i = 0; i < tasks->nbTask; i++){
        cleanTask(&tasks->tasks[i]);
    }
    for (i = 0; i < actions->nbAction; i++){
        cleanAction(&actions->actions[i]);
    }
    free(tasks->tasks);
    free(actions->actions);
    tasks->tasks = NULL;
    actions->actions = NULL;
}

int readConf(char* confPath, ListTask *tasks, ListAction *actionsList){
    FILE* fp = fopen(confPath, "r");
    int nbCharRead = 0;
    char * actualLine = malloc(sizeof(char)*250);
    
    if (fp != NULL && actualLine != NULL){
        while (getOneLine(actualLine, fp)){
            nbCharRead = nbCharRead + (int)strlen(actualLine);
            removeCommentFromLine(actualLine);
            
            if(strcmp(actualLine,"=") == 0){                            // Création d'une action
                Action newAction;
                initAction(&newAction);
                if (!completeAction(&newAction, fp, &nbCharRead)){      // Le ficher de conf n'est pas valide pour la créationd de l'action
                    fprintf(stderr,"Fichier de configuration erroné.\n");
                    return 0;
                }
                actionsList->actions[actionsList->nbAction] = newAction;
                actionsList->nbAction ++;
                
                fseek(fp, nbCharRead, SEEK_SET);                        // On veut retourner à la ligne précédente donc on part du début du fichier et on se positionne au nombre de caractère déjà lu
            }
            
            else if (strcmp(actualLine,"==") == 0){                     // Création d'une tâche
                Task newTask;
                initTask(&newTask);
                if (!completeTask(&newTask, fp, &nbCharRead, actionsList)){
                    fprintf(stderr,"Fichier de configuration erroné.\n");
                    return 0;
                }
                tasks->tasks[tasks->nbTask] = newTask;
                tasks->nbTask ++;
                fseek(fp, nbCharRead, SEEK_SET);
            }
            
        }
        fclose(fp);
    } else {
        fprintf(stderr,"Erreur lors de l'initialisation.");
        return 0;
    }
    free(actualLine);
    return 1;
}

void removeCommentFromLine(char *line){
    int i, startComment=(int)strlen(line);
    char *tmpLine;
    for (i = 0; i < strlen(line); i++){
        if (line[i] == '#'){
            startComment = i;
        }
    }
    if (startComment == (int)strlen(line)) { // Pas de commentaire donc rien à faire
        return;
    }

    // S'il existe un commentaire dans la ligne on copie la ligne sans le commentaire
    // dans la variable temporaire
    tmpLine = malloc(sizeof(char)*startComment);
    strncpy(tmpLine, line, startComment);
    strncpy(line, tmpLine, startComment);
    tmpLine[startComment-1] = '\0';
}



int getOneLine(char *line, FILE *fp){
    if (fp != NULL){
        if (fgets(line, 250, fp) != NULL){
            if (strlen(line)<250){
                line[strlen(line)-1] = '\0';
            }
            return 1;
        }
    }
    return 0;
}

void cleanLine(char *line){
    int indexStartKeeping = 0, indexEndKipping = (int)strlen(line), index = 0;
    char *tmpLine = malloc(sizeof(char)*(int)strlen(line));
    if (tmpLine == NULL){
        return;
    }
    while (index < strlen(line)){
        if (line[index] == '{'){
            indexStartKeeping = index;
        }
        if (line[index] == '}'){
            indexEndKipping = index;
        }
        index ++;
    }
    
    if (indexEndKipping != (int)strlen(line)){
        strncpy(tmpLine, line+indexStartKeeping, indexEndKipping-indexStartKeeping+1);
        strcpy(line, tmpLine);
        line[strlen(tmpLine)] = '\0';

    }
    free(tmpLine);
}

void getAttributName(char *actualLine, char *actualAttributName, int *index){ // Récupère les attributs avec des lignes "{attribut -> valeur}"
    int copyIndex = *index;
    int indexAttributName = 0;
    
    if (actualLine[copyIndex] == '{'){
        copyIndex ++;
    } else {
        fprintf(stderr,"ERREUR\n");
        return;
    }
    if (actualAttributName[copyIndex] == ' '){
        copyIndex ++;
    }
    while (actualLine[copyIndex] != ' '){
        if (copyIndex <2500 && actualLine[copyIndex] == '-' && actualLine[copyIndex+1] == '>'){
            break;
        }
        actualAttributName[indexAttributName] = actualLine[copyIndex];
        copyIndex ++;
        indexAttributName ++;
    }
    if (indexAttributName < 20){
        actualAttributName[indexAttributName] = '\0';
    }
    *index = copyIndex;
}

void getAttributValueInt(char *actualLine, int *actualAttributValueInt, int index){
    index ++;
    while (actualLine[index] == '-' || actualLine[index] == '>'){
        index ++;
    }
    if (actualLine[index] == ' '){
        index ++;
    }
    *actualAttributValueInt = actualLine[index];
}


void getAttributValue(char *actualLine, char *actualAttributValue, int index){
    int indexAttributValue = 0;
    index ++;
    while (actualLine[index] == '-' || actualLine[index] == '>'){
        index ++;
    }
    if (actualLine[index] == ' '){
        index ++;
    }
    while (index < 2500 && actualLine[index] != '}'){
        actualAttributValue[indexAttributValue] = actualLine[index];
        indexAttributValue ++;
        index ++;
    }
    if (indexAttributValue < 2000){
        actualAttributValue[indexAttributValue] = '\0';
    }
}

void getAttributValueType(char *actualLine,char **actualAttributValue, int index, int *sizeTabList){
    index ++;
    int counterTabList = 0;
    int counterInWord = 0;
    while (actualLine[index] == '-' || actualLine[index] == '>'){
        index ++;
    }
    if (actualLine[index] == ' '){
        index ++;
    }
    if (actualLine[index] == '('){
        index ++;
    }
    while (index < 2500 && actualLine[index] != ')'){ // Boucle pour tous les mots
        counterInWord = 0;
        while(index < 2500 && actualLine[index] != ','){ // Boucle pour un mot
            if (actualLine[index] == ')'){
                break;
            }
            actualAttributValue[counterTabList][counterInWord] = actualLine[index];
            counterInWord ++;
            index ++;
        }
        if (counterInWord < 2000){
            actualAttributValue[counterTabList][counterInWord] = '\0';
        }
        if (actualLine[index] == ','){
            index ++;
        }
        counterTabList ++;
    }
    *sizeTabList = counterTabList;
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




