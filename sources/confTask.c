//
//  confTask.c
//  sCrapper
//
//  Created by CELIA PIQUET on 13/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

ListTask *getAllTasksFromConnf(FILE *fp, ListAction *allActions){
    fseek(fp, 0, SEEK_SET); // On se replace au début du fichier
    char *actualLine = malloc(sizeof(char)*SIZE_MAX_OF_LINE_CONF);
    int charReaden;
    Task *taskToAdd = NULL;
    
    ListTask *allTasks = initListTask();
    if (allTasks == NULL){
        free(actualLine);
        return NULL;
    }
    
    while (getOneLine(actualLine, fp, &charReaden, SIZE_MAX_OF_LINE_CONF)){ // On lit chaque ligne
        cleanOneLine(actualLine);       // On nettoie la ligne des espaces superflues et des commentaires
        if(strcmp(actualLine,"==") == 0){
            taskToAdd = createOneTaskFromConf(fp, &charReaden, allActions);
            if (taskToAdd == NULL){
                return NULL;
            }
            if(!addTaskToList(allTasks, taskToAdd)){
                return NULL;
            }
            fseek(fp, charReaden, SEEK_SET); // Permet de nous replacer juste avant la déclaration d'une action ou tache
        }
    }
    return allTasks;
}

Task *createOneTaskFromConf(FILE *fp, int  *charReaden, ListAction *allActions){
    char *actualLine = malloc(sizeof(char)*SIZE_MAX_OF_LINE_CONF);
    if (actualLine == NULL){
        return NULL;
    }
    Task *newTask = initTask();
    if(newTask == NULL){
        free(actualLine);
        return NULL;
    }
    
    while(getOneLine(actualLine, fp, charReaden, SIZE_MAX_OF_LINE_CONF) && actualLine[0] != '='){
        cleanOneLine(actualLine);
        if (strlen(actualLine) == 0 || actualLine[0] == '+'){ // On ignore ces lignes
            continue;
        }
        if(!completeTaskAttribut(newTask,actualLine, allActions)){ // L'attribut ou sa valeur ne sont pas bons, fichier corrompu
            free(actualLine);
            return NULL;
        }
    }
    free(actualLine);
    return newTask;
}

int getActionName(char *actualLine, char *actionName){
    int index = 0;
    actionName[0] = '\0';
    while (index < strlen(actualLine) && actualLine[index] != ',' && actualLine[index] != ')' && index-1 < SIZE_MAX_ATTRIBUT_VALUE){
        actionName[index] = actualLine[index];
        index ++;
    }
    if (index != SIZE_MAX_ATTRIBUT_VALUE){
        actionName[index] = '\0';
    }
    if (index == strlen(actualLine)){
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

int addActionToTask(Task *task, Action *action){
    if (task == NULL || action == NULL || task->actionsToRun.nbOfAction == task->actionsToRun.capacity){
        return 0;
    }
    task->actionsToRun.tabAction[task->actionsToRun.nbOfAction] = *action;
    task->actionsToRun.nbOfAction ++;
    return 1;
}

int setActionListToTask(Task *task, char *actualLine, ListAction* allActions){
    char *actionName = malloc(sizeof(char)*SIZE_MAX_ATTRIBUT_VALUE);
    int charReaden = 1;
    if (actionName == NULL){
        return 0;
    }
    char *tmpLine = malloc(sizeof(char)*strlen(actualLine));
    if (tmpLine == NULL){
        free(actionName);
        return 0;
    }
    Action *actualAction = NULL;
    strcpy(tmpLine, actualLine+charReaden);
    
    while(getActionName(tmpLine, actionName)){
        actualAction = getActionFromList(actionName, allActions);
        charReaden = charReaden + (int)strlen(actionName) + 1;
        if (actualAction != NULL){
            if (!addActionToTask(task, actualAction)){
                free(actionName);
                free(tmpLine);
                return 0;
            }
        } else {
            free(actionName);
            free(tmpLine);
            return 0;
        }
        strcpy(tmpLine, actualLine+charReaden);
    }
    free(actionName);
    free(tmpLine);
    return 1;
}

int initTaskAttributVariables(char **attributName, char **charAttribut, char **charAttributForInt){
    *attributName = malloc(sizeof(char)*15);
    if (*attributName == NULL){
        return 0;
    }
    
    *charAttribut = malloc(sizeof(char)*1000);
    if(*charAttribut == NULL){
        free(*attributName);
        return 0;
    }
    
    *charAttributForInt = malloc(sizeof(char)*2); // On ne veut pas un int supérieur à 99
    if (*charAttributForInt == NULL){
        free(attributName);
        free(charAttribut);
        return 0;
    }
    return 1;
}

void cleanTaskAttributVariables(char **attributName, char **charAttribut, char **attributValueInt){
    if (*attributName != NULL){
        free(*attributName);
    }
    if(*charAttribut != NULL){
        free(*charAttribut);
    }
}

int completeTaskAttribut(Task *task, char* actualLine, ListAction *allActions){
    char *attributValueInt = NULL, *attributName = NULL, *attributValueStr = NULL;

    if(!initTaskAttributVariables(&attributName, &attributValueStr, &attributValueInt)){
         return 0;
     }
    
    if (actualLine[0] == '('){
        return setActionListToTask(task, actualLine, allActions);
    }
    
    if (actualLine[0] != '{' || actualLine[strlen(actualLine)-1] != '}'){ // on veut que la ligne soit de la forme {nom_de_lattribut -> valeure_de_lattribut}
        cleanTaskAttributVariables(&attributName, &attributValueStr, &attributValueInt);
        return 0;
    }
    if (!getAttributName(attributName, actualLine)){
        cleanTaskAttributVariables(&attributName, &attributValueStr, &attributValueInt);
        return 0;
    }
     
    if (strcmp(attributName,"name") == 0){                                                  //la valeur de l'attribue sera une chaine de caractères
        getAttributValueStr(actualLine, attributValueStr, (int)strlen(attributName)+1, SIZE_MAX_ATTRIBUT_VALUE);
        setTaskAttributStr(attributName, task, attributValueStr);
    } else {                                                                                // la valeur sera un entier
        getAttributValueStr(actualLine, attributValueInt, (int)strlen(attributName)+1, 3);
        setTaskAttributInt(attributName, task, attributValueInt);
    }
    
    cleanTaskAttributVariables(&attributName, &attributValueStr, &attributValueInt);
    return 1;
}

int addTaskToList(ListTask *allTasks, Task *taskToAdd){
    if (allTasks == NULL || taskToAdd == NULL || allTasks->capacity == allTasks->nbOfTask){
        return 0;
    }
    allTasks->tabTask[allTasks->nbOfTask] = *taskToAdd;
    allTasks->nbOfTask ++;
    return 1;
}

void setTaskAttributStr(char *attributName, Task *task, char *attributValueStr){
    if (strcmp(attributName, "name") == 0){
        strcpy(task->name, attributValueStr);
    }
}

void setTaskAttributInt(char *attributName, Task *task, char *attributValueInt){
    if (strcmp(attributName, "second") == 0){
        task->second = atoi(attributValueInt);
    } else if (strcmp(attributName,"hour") == 0){
        task->hour = atoi(attributValueInt);
    } else if (strcmp(attributName,"minute") == 0){
        task->minute = atoi(attributValueInt);
    }
}

void displayTask(Task task){
    fprintf(stderr, "TASK : {\n");
    fprintf(stderr, "    Name : %s\n", task.name);
    fprintf(stderr, "    Minute : %d\n", task.minute);
    fprintf(stderr, "    Second : %d\n", task.second);
    fprintf(stderr, "    Hour : %d\n", task.hour);
    fprintf(stderr, "    Actions : \n");
    for (int i=0; i<task.actionsToRun.nbOfAction; i++){
        displayAction(task.actionsToRun.tabAction[i]);
    }
    fprintf(stderr, "}\n");
}
