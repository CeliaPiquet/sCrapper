//
//  confTask.h
//  sCrapper
//
//  Created by CELIA PIQUET on 13/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confTask_h
#define confTask_h

    Task* initTask(void);
    ListTask *getAllTasksFromConnf(FILE *fp, ListAction *allActions);
    Task *createOneTaskFromConf(FILE *fp, int  *charReaden, ListAction *allActions);
    int addTaskToList(ListTask *allTasks, Task *taskToAdd);
    int completeTaskAttribut(Task *task, char* actualLine, ListAction *allActions);
    void setTaskAttributStr(char *attributName, Task *task, char *attributValueStr);
    void setTaskAttributInt(char *attributName, Task *task, char *attributValueInt);
    void cleanTaskAttributVariables(char **attributName, char **charAttribut, char **attributValueInt);
    void displayTask(Task task);
    int setActionListToTask(Task *task, char *actualLine, ListAction* allActions);
    ListTask* initListTask(int capacity);

#endif /* confTask_h */
