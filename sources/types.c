//
//  types.c
//  sCrapper
//
//  Created by CELIA PIQUET on 13/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

Task* initTask(void){
    Task *task = malloc(sizeof(Task));
    if (task == NULL){
        return NULL;
    }
    task->name = malloc(sizeof(char)*SIZE_MAX_STR_ATTRIBUT);
    if (task->name == NULL){
        free(task);
        return NULL;
    }
    ListAction *listActions = initListAction(NB_MAX_OF_ACTION_PER_TASK);
    if (listActions == NULL){
        free(task->name);
        free(task);
        return NULL;
    }
    task->actionsToRun = *listActions;
    task->hour = 0;
    task->minute = 0;
    task->second = 0;
    return task;
}

ListTask* initListTask(int capacity){
    ListTask *list = malloc(sizeof(ListTask));
    if (list == NULL){
        return NULL;
    }
    list->capacity = capacity;
    list->nbOfTask = 0;
    list->tabTask = malloc(sizeof(Task)*list->capacity);
    if (list->tabTask == NULL){
        free(list);
        return NULL;
    }
    return list;
}

Action* initAction(void){
    Action *action = malloc(sizeof(Action));
    if (action == NULL){
        return NULL;
    }
    action->hasVersionning = 0;
    action->maxDepth = 0;
    action->name = malloc(sizeof(char)*SIZE_MAX_STR_ATTRIBUT);
    if (action->name == NULL){
        free(action);
        return NULL;
    }
    action->url = malloc(sizeof(char)*SIZE_MAX_URL);
    if(action->url == NULL){
        free(action->name);
        free(action);
        return NULL;
    }
    ListLinks *listLinks = initListLinks(INITIAL_NB_OF_LINKS_PER_ACTION);
    if (listLinks == NULL){
        free(action->name);
        free(action->url);
        free(action);
        return NULL;
    }
    action->allUrlsWithDepth = *listLinks;

    ListType *listType = initListType(NB_MAX_OF_TYPES_PER_ACTION);
    if (listType == NULL){
        freeListLinks(&action->allUrlsWithDepth);
        free(action->name);
        free(action->url);
        free(action);
        return NULL;
    }
    action->typesToTarget = *listType;
    return action;
}

ListType* initListType(int capacity){
    ListType *list = malloc(sizeof(ListType));
    if (list == NULL){
        return NULL;
    }
    list->capacity = capacity;
    list->nbOfType = 0;
    list->tabType = malloc(sizeof(char*)*list->capacity);
    if (list->tabType == NULL){
        free(list);
        return NULL;
    }
    for (int i = 0; i < list->capacity; i++){
        list->tabType[i] = malloc(sizeof(char)*SIZE_MAX_STR_ATTRIBUT);
        if (list->tabType[i] == NULL){
            for (int j = i-1; j >= 0; j--){
                free(list->tabType[j]);
            }
            free(list->tabType);
            free(list);
            return NULL;
        }
    }
    return list;
}

ListLinks* initListLinks(int capacity){
    ListLinks *list = malloc(sizeof(ListLinks));
    if (list == NULL){
        return NULL;
    }
    list->capacity = capacity;
    list->nbOfUrl = 0;
    list->tabUrls = malloc(sizeof(char*)*list->capacity);
    if(list->tabUrls == NULL){
        free(list);
        return NULL;
    }
    for (int i = 0; i < list->capacity; i++){
        list->tabUrls[i] = malloc(sizeof(char)*SIZE_MAX_URL);
        if (list->tabUrls[i] == NULL){
            for (int j = i-1; j >= 0; j--){
                free(list->tabUrls[j]);
            }
            free(list->tabUrls);
            free(list);
            return NULL;
        }
    }
    return list;
}

ListAction* initListAction(int capacity){
    ListAction *list = malloc(sizeof(ListAction));
    if (list == NULL){
        return NULL;
    }
    list->capacity = capacity;
    list->nbOfAction = 0;
    list->tabAction = malloc(sizeof(Action)*list->capacity);
    if (list->tabAction == NULL){
        free(list);
        return NULL;
    }
    return list;
}


void freeListLinks(ListLinks *list){
    if (list != NULL){
        if (list->tabUrls != NULL){
            for (int i = 0; i < list->capacity; i++){
                if (list->tabUrls[i] != NULL){
                    free(list->tabUrls[i]);
                }
            }
            free(list->tabUrls);
        }
        free(list);
    }
}
