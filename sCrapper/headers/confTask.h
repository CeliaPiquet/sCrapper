//
//  confTask.h
//  sCrapper
//
//  Created by CELIA PIQUET on 17/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confTask_h
#define confTask_h
    #include "types.h"
    #include "scrapper.h"
    #include <stdio.h>

    int completeTask(Task *newTask, FILE *fp, int *nbChar, ListAction *actionsList);
    void cleanTask(Task *task);
    void initTask(Task *task);
    void displayTask(Task *task);
    void setTaskAttributInt(char *attributName, Task *task, int attributValueInt);
    void setTaskAttribut(char *attributName, Task *task, char *attributValue);
    int setActionListToTask(Task *task, ListAction *actionsList, char *actualLine);

#endif /* confTask_h */
