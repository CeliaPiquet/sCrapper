//
//  confAction.h
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confAction_h
#define confAction_h

    ListType* initListType(void);
    ListAction* initListAction(int capacity);
    ListAction* getAllActionsFromConf(FILE *fp);
    Action* createOneActionFromConf(FILE *fp, int *charReaden);
    Action* initAction(void);
    int completeActionAttribut(Action *action, char *actualLine);
    void displayAction(Action action);
    int addActionToList(ListAction *listAction, Action *actionToAdd);
    int setActionAttributTabStr(char *attributName, Action *action, char **attributValueTabStr, int sizeOfTabStr);
    void cleanListAction(ListAction *allActions);
    void cleanAction(Action *action);
    int setActionAttributInt(char *attributName, Action *action, char *attributValueInt);
    void setActionAttributStr(char *attributName, Action *action, char *attributValue);
    void cleanActionAttributVariables(char **attributName, char **charAttribut, char ***stringTabAttribut, char **attributValueInt);

#endif /* confAction_h */
