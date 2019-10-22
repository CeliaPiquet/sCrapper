//
//  scrapper.h
//  sCrapper
//
//  Created by CELIA PIQUET on 15/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef scrapper_h
#define scrapper_h

    #include <stdio.h>
    #include <curl/curl.h>
    #include <stdlib.h>
    #include <string.h>

    typedef struct Action Action;
    struct Action {
        char* name;
        char* url;
        int maxDepth;
        int versionning; // bool : 0 or 1
        int sizeTypeList;
        char** listType;
    };

    typedef struct Task Task;
    struct Task {
        char *name;
        int sizeListAction;
        int minute;
        int second;
        int hour;
        Action * listActions;
    };

    // Permet de créer et compléter un objet Action en fonction de la conf
    int completeAction(Action *action, FILE *fp, int *nbChar);
    int completeTask(Task *newTask, FILE *fp, int *nbChar, Action *listActions, int nbAction);

    // Première fonction qui ouvre le fichier de conf, elle initie le reste
    int readConf(char* confPath);
    
    // Permet de récupérer une ligne du fichier bien formaté au niveau de la taille etc..
    int getOneLine(char *line, FILE *fp);
    
    // Permet de gérer nos objets Action et Task : initialisation , suppression, affichage
    void initAction(Action *action);
    void displayAction(Action *action);
    void initTask(Task *task);
    void displayTask(Task *task);

    
    // Permet de modifier les attribut de nos objets Action et Task
    void setActionAttribut(char *attributName, Action *action, char *attributValue);
    void setActionAttributInt(char *attributName, Action *action, int attributValueInt);
    void setActionAttributType(Action *action, char **tabList, int sizeTabList);
    void setTaskAttributInt(char *attributName, Task *task, int attributValueInt);
    void setTaskAttribut(char *attributName, Task *task, char *attributValue);

    // Permet de récupérer le nom de l'attribut que l'on veut modifier dans notre objet Action
    void getAttributName(char *actualLine, char *actualAttributName, int *index);

    // Permet de récupérer la ou les valeur pour un attribut / une ligne
    void getAttributValueType(char *actualLine,char **actualAttribut, int index, int *sizeTabList);
    void getAttributValueInt(char *actualLine, int *actualAttributValueInt, int index);
    void getAttributValue(char *actualLine, char *actualAttributValue, int index);

    // vérifie si l'attribut est bien parmi ceux autorisés
int isCorrectAttributName(char *attributName);

void removeCommentFromLine(char *line);
int getActionName(char *line, char *actionName);
int setActionListToTask(Task *task, Action *listActions, int nbAction, char *actualLine);
int getActionFromList(Action *listActions, char *actionName, int nbAction, Action *actionToUpdate);
void cleanLine(char *line);

#endif /* scrapper_h */
