//
//  confTask.c
//  sCrapper
//
//  Created by CELIA PIQUET on 17/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../Headers/scrapper.h"

int completeTask(Task *task, FILE *fp, int *nbChar, Action *listActions, int nbAction){
    int index = 0;
    char *actualLine = malloc(sizeof(char)*2500);
    char *actualAttributName = malloc(sizeof(char)*20);
    char *actualAttributValue = malloc(sizeof(char)*2000);
    int actualAttributValueInt;
    
    while (getOneLine(actualLine, fp) && actualLine[0] != '='){
        
        *nbChar = *nbChar + (int)strlen(actualLine);
        if (actualLine[0] == '#' || actualLine[0] == '+' || strlen(actualLine) == 0){
            continue; // On ignore
        }
        if (actualLine[0] == '('){
            // Liste des actions
            if (!setActionListToTask(task, listActions, nbAction, actualLine)){
                return 0;
            }
            
        } else {
            // On récupère l'attribut de la ligne et on le vérifie
            getAttributName(actualLine, actualAttributName, &index);
            if (!isCorrectAttributName(actualAttributName)){
                printf("Attribut '%s' incorrect\n", actualAttributName);
                return 0;
            }
            
            // Si l'attribut est "name" on gère des chaines de caractères, sinon des entiers
            if(!strcmp("name", actualAttributName)){
                getAttributValue(actualLine, actualAttributValue, index);
                setTaskAttribut(actualAttributName, task, actualAttributValue);
            } else {
                getAttributValueInt(actualLine, &actualAttributValueInt, index);
                setTaskAttributInt(actualAttributName, task, actualAttributValueInt);
            }
        }
        
        index  = 0;
    }
    free(actualLine);
    free(actualAttributName);
    free(actualAttributValue);
    return 1;
}



void initTask(Task *task){
    task->hour = 0;
    task->minute = 0;
    task->second = 0;
    task->name = malloc(sizeof(char)*20);
    task->sizeListAction = 0;
    task->listActions = malloc(sizeof(char)*10);
}

int setActionListToTask(Task *task, Action *listActions, int nbAction, char *actualLine){
    char *actionName = malloc(sizeof(char)*20);
    if (!getActionName(actualLine, actionName)){
        printf("Il faut renseigner au moins une action pour la création de tâche.\n");
        return 0;
    }
    if (task->sizeListAction >= 10){
        printf("La tâche a déjà 10 actions, créez une autre tâche pour les autres.\n");
        return 0;
    }
    
    if (!getActionFromList(listActions, actionName, nbAction, &task->listActions[task->sizeListAction])){
        printf("Aucune action nommée '%s' n'a été trouvée.\n", actionName);
        return 0;
    }
    task->sizeListAction ++;
    
    return 1;
}

int getActionFromList(Action *listActions, char *actionName, int nbAction, Action* actionToUpdate){
    int i;
    for (i = 0; i < nbAction; i++){
        if (strcmp(listActions[i].name, actionName) == 0){
            *actionToUpdate = listActions[i];
            return 1;
        }
    }
    return 0;
}

void displayTask(Task *task){
    int i, j;
    printf("TASK : {\n");
    printf("    Name : %s\n", task->name);
    printf("    Minute : %d\n", task->minute);
    printf("    Second : %d\n", task->second);
    printf("    Hour : %d\n", task->hour);
    printf("    Action(s) :\n");
    for (i = 0; i < task->sizeListAction; i++){
        printf("        {\n");
        printf("            Name : %s\n", task->listActions[i].name);
        printf("            Url : %s\n", task->listActions[i].url);
        printf("            Max-depth : %d\n", task->listActions[i].maxDepth);
        printf("            Versioning : %d\n", task->listActions[i].versionning);
        printf("            Type :\n");
        for (j = 0; j < task->listActions[i].sizeTypeList; j++){
            printf("                %s\n", task->listActions[i].listType[j]);
        }
        printf("        },\n");
    }
    printf("}\n");
}
int getActionName(char *line, char *actionName){
    int index = 1; // On commence juste après la parenthèse
    while (index < strlen(line) && line[index] != ',' && line[index] != ')'){
        actionName[index-1] = line[index];
        index ++;
    }
    actionName[index-1] = '\0';
    if (index == strlen(line)){
        printf("\nMauvais formatage de la liste d'action\n");
        return 0;
    }
    return 1;
}
void setTaskAttributInt(char *attributName, Task *task, int attributValueInt){
    int value = attributValueInt%48;                        // %48 pour passer de char à int A MODIFIER
    if (strcmp("second", attributName) == 0){
        task->second = value;
    } else if (strcmp("minute", attributName) == 0){
        task->minute = value;
    } else if (strcmp("hour", attributName) == 0){
        task->hour = value;
    }
}

void setTaskAttribut(char *attributName, Task *task, char *attributValue){
    if (strcmp("name", attributName) == 0){
        strcpy(task->name, attributValue);
    }
}
