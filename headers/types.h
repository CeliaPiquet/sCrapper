//
//  types.h
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef types_h
#define types_h
    
    typedef struct ListType ListType;
    struct ListType {
        int capacity;
        int nbOfType;
        char **tabType;
    };

    typedef struct ListLinks ListLinks;
    struct ListLinks{
        char **tabUrls;
        int capacity;
        int nbOfUrl;
    };

    typedef struct Action Action;
    struct Action {
        char *name;
        char *url;
        int maxDepth;
        int hasVersionning;
        ListType typesToTarget;
        ListLinks allUrlsWithDepth;
    };


    typedef struct ListAction ListAction;
    struct ListAction {
        int capacity;
        int nbOfAction;
        Action *tabAction;
    };

    typedef struct Task Task;
    struct Task {
        char *name;
        int hour;
        int minute;
        int second;
        ListAction actionsToRun;
    };

    typedef struct ListTask ListTask;
    struct ListTask {
        int capacity;
        int nbOfTask;
        Task *tabTask;
    };

#endif /* types_h */
