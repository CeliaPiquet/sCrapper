//
//  confAction.h
//  sCrapper
//
//  Created by CELIA PIQUET on 17/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confAction_h
#define confAction_h
    #include "types.h"
    #include "scrapper.h"
    #include <stdio.h>
    
    void cleanAction(Action *action);
    int completeAction(Action *action, FILE *fp, int *nbChar);
    void initAction(Action *action);
    void displayAction(Action action);
    void setActionAttribut(char *attributName, Action *action, char *attributValue);
    void setActionAttributInt(char *attributName, Action *action, int attributValueInt);
    void setActionAttributType(Action *action, char **tabList, int sizeTabList);
    int getActionName(char *line, char *actionName);
    int getActionFromList(ListAction *actionsList, char *actionName, Action *actionToUpdate);
    void displayActionList(ListAction *actionList);
#endif /* confAction_h */
