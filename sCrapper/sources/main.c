//
//  main.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//
#include "../headers/scrapper.h"

int main(int argc, const char * argv[]) {
    
    int exitCode;

    // On initialise la liste des tâches qui nous sevrira pour la suite
    ListTask tasks;
    tasks.sizeMax = 5; //On se limite initiallement à 5 tâches
    tasks.tasks = malloc(sizeof(Task)*tasks.sizeMax);
    tasks.nbTask = 0;

    ListAction actionList;
    actionList.sizeMax = 10;
    actionList.actions = malloc(sizeof(Action)*actionList.sizeMax);
    actionList.nbAction = 0;

    readConf("/Users/cpiquet/dev/projets_esgi/sCrapper/sCrapper/resources/conf.sconf", &tasks, &actionList);

    // display for check
    fprintf(stderr,"%d tasks\n", tasks.nbTask);
    for (int i=0; i<tasks.nbTask; i++){
        displayTask(&tasks.tasks[i]);
    }

    exitCode = startScrapping(tasks);
    printf("Exit code : %d\n", exitCode);

    //cleanConf(&tasks, &actionList);
    return 0;
}
