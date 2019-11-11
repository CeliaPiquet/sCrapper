//
//  scrapper.h
//  sCrapper
//
//  Created by CELIA PIQUET on 15/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef scrapper_h
#define scrapper_h
    #include "confTask.h"
    #include "confAction.h"
    #include "urlParser.h"
    #include <stdio.h>
    #include <curl/curl.h>
    #include <stdlib.h>
    #include <string.h>
    #include <pthread.h>
    #include <errno.h>
    #include <unistd.h>
    #include <sys/stat.h>

    #define PARENT_PATH "/Users/cpiquet/dev/projets_esgi/sCrapper/sCrapper/"

    
    // Fonctions confParser.c
     int readConf(char* confPath, ListTask *tasks, ListAction *actionsList);
     void cleanConf(ListTask *tasks, ListAction *actions);
     int getOneLine(char *line, FILE *fp);
     void getAttributName(char *actualLine, char *actualAttributName, int *index);
     void getAttributValueType(char *actualLine,char **actualAttribut, int index, int *sizeTabList);
     void getAttributValueInt(char *actualLine, int *actualAttributValueInt, int index);
     void getAttributValue(char *actualLine, char *actualAttributValue, int index);
     int isCorrectAttributName(char *attributName);
     void removeCommentFromLine(char *line);
     void cleanLine(char *line);
    

 

#endif /* scrapper_h */
