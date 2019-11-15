//
//  confAction.h
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confAction_h
#define confAction_h

    ListType* initListType(int capacity);

    ListAction* initListAction(int capacity);
    ListAction* getAllActionsFromConf(FILE *fp);

    Action* createOneActionFromConf(FILE *fp, int *charReaden);
    Action* initAction(void);
    
    int completeActionAttribut(Action *action, char *actualLine);
    int addActionToList(ListAction *listAction, Action *actionToAdd);
    int setActionAttributTabStr(char *attributName, Action *action, char **attributValueTabStr, int sizeOfTabStr);
    int setActionAttributInt(char *attributName, Action *action, char *attributValueInt);
    
    void cleanAction(Action *action);
    void cleanListAction(ListAction *allActions);
    void setActionAttributStr(char *attributName, Action *action, char *attributValue);
    void cleanActionAttributVariables(char **attributName, char **charAttribut, char ***stringTabAttribut, char **attributValueInt);
    void displayAction(Action action);

#endif /* confAction_h */
