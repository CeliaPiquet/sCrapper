//
//  confTask.c
//  sCrapper
//
//  Created by CELIA PIQUET on 17/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/scrapper.h"

void cleanTask(Task *task){
    free(task->name);
    task->name = NULL;
    free(task->actionsList.actions);
    task->actionsList.actions = NULL;
}

int completeTask(Task *task, FILE *fp, int *nbChar, ListAction *actionsList){
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
            if (!setActionListToTask(task, actionsList, actualLine)){
                return 0;
            }
            
        } else {
            // On récupère l'attribut de la ligne et on le vérifie
            getAttributName(actualLine, actualAttributName, &index);
            if (!isCorrectAttributName(actualAttributName)){
                fprintf(stderr, "Attribut '%s' incorrect\n", actualAttributName);
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
    ListAction actionList;
    actionList.sizeMax = 5; //On se limite à 5 actions par tâche
    actionList.actions = malloc(sizeof(Action)*actionList.sizeMax);
    actionList.nbAction = 0;
 
    task->name = malloc(sizeof(char)*20);
    task->hour = 0;
    task->minute = 0;
    task->second = 0;
    task->actionsList = actionList;
}

int setActionListToTask(Task *task, ListAction *actionList, char *actualLine){
    char *actionName = malloc(sizeof(char)*20);
    char *lineTmp = malloc(sizeof(char)*strlen(actualLine));
    int sizeActionName = 0;
    int nbOfActionInTask = task->actionsList.nbAction;
    strcpy(lineTmp, actualLine+sizeActionName);
    
    while (getActionName(lineTmp, actionName)){
        if ( nbOfActionInTask >= task->actionsList.sizeMax){
            fprintf(stderr, "La tâche a déjà son nombre d'actions maximum autorisé (5).\nCréez une autre tâche pour les autres actions.\n");
            return 0;
        }
        if (!getActionFromList(actionList, actionName,&task->actionsList.actions[nbOfActionInTask])){
            fprintf(stderr, "Aucune action nommée '%s' n'a été trouvée.\n", actionName);
            return 0;
        }
        task->actionsList.nbAction ++;
        nbOfActionInTask ++;
        sizeActionName = sizeActionName + (int)strlen(actionName) + 1;
        strcpy(lineTmp, actualLine+sizeActionName);
    }
    
    if (task->actionsList.nbAction == 0){
        fprintf(stderr, "Il faut renseigner au moins une action pour la création de tâche.\n");
        return 0;
    }
    
    return 1;
}

int getActionFromList(ListAction *actionList, char *actionName, Action* actionToUpdate){
    int i;
    int nbAction = actionList->nbAction;
    for (i = 0; i < nbAction; i++){
        if (strcmp(actionList->actions[i].name, actionName) == 0){
            *actionToUpdate = actionList->actions[i];
            return 1;
        }
    }
    return 0;
}

void displayTask(Task *task){
    fprintf(stderr, "TASK : {\n");
    fprintf(stderr, "    Name : %s\n", task->name);
    fprintf(stderr, "    Minute : %d\n", task->minute);
    fprintf(stderr, "    Second : %d\n", task->second);
    fprintf(stderr, "    Hour : %d\n", task->hour);
    displayActionList(&task->actionsList);
    fprintf(stderr, "}\n");
}
int getActionName(char *line, char *actionName){
    int index = 1; // On commence juste après la parenthèse
    while (index < strlen(line) && line[index] != ',' && line[index] != ')'){
        actionName[index-1] = line[index];
        index ++;
    }
    actionName[index-1] = '\0';
    if (index == strlen(line)){
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
