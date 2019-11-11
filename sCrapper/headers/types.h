//
//  types.h
//  sCrapper
//
//  Created by CELIA PIQUET on 06/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef types_h
#define types_h
#include <pthread.h>

    typedef struct ListLinks ListLinks;
    struct ListLinks{
        char** listUrls;
        int capacity;
        int size;
    };

    typedef struct Action Action;
    struct Action {
        char* name;
        char* url;
        int maxDepth;
        int hasVersionning;
        int sizeTypeList;   // Le nombre de type maximum pour une action
        int nbType;         // Le nombre réel de type pour l'action en cours
        char** typeList;
        ListLinks *urlsWithDepth; // La liste de toutes les urls à parser pour cette action, dépend de maxDepth
    };

    typedef struct ListAction ListAction;
    struct ListAction{
        Action *actions;
        int sizeMax;
        int nbAction;
    };
    
    typedef struct Task Task;
    struct Task {
        char *name;
        int hour;
        int minute;
        int second;
        ListAction actionsList;
    };

    typedef struct ListTask ListTask;
    struct ListTask{
        Task *tasks;
        int sizeMax;
        int nbTask;
    };

    typedef struct ListThread ListThread;
    struct ListThread{ 
        pthread_t *threads;
        int sizeMax;
        int nbThreads;
    };

   

#endif /* types_h */
