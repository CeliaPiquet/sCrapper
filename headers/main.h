//
//  main.h
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef main_h
#define main_h
    #include <stdio.h>
    #include <stdlib.h>
    #include <curl/curl.h>
    #include <string.h>
    #include <pthread.h>
    #include <errno.h>
    #include <unistd.h>
    #include <sys/stat.h>
    #include "types.h"
    #include "confParser.h"
    #include "confAction.h"
    #include "confListLinks.h"
    #include "confTask.h"
    #include "curlFunctions.h"
    #include "scrapping.h"

    #define SIZE_MAX_STR_ATTRIBUT   30                  //La taille max d'un name pour une action ou une tâche
    #define SIZE_MAX_URL  2083                          //La taille max d'une url
    #define SIZE_MAX_OF_LINE_IN_CONF 1000               //La taille max d'une ligne du fichier de conf du scrapper
    #define SIZE_MAX_OF_LINE_DOWNLOADS  3000            //La taille max d'une ligne d'un fichier téléchargé
    #define NB_MAX_OF_ACTION_PER_TASK   5               //Nombre d'actions maximum par tâche
    #define NB_MAX_OF_ACTIONS_IN_CONF   10              //Nombre d'actions maximum dans la conf
    #define NB_MAX_OF_TYPES_PER_ACTION  5               //Nombre de types max par action
    #define INITIAL_NB_OF_LINKS_PER_ACTION  1000        //Nombre de liens max initial par tâche (ce max peut changer durant le programme)
    #define SIZE_MAX_TASK_LIST  5                      //Taille maximale de la liste de tâches
    #define PARENT_PATH "/Users/cpiquet/dev/esgi/sCrapper/sCrapper/"
    #define CONF_PATH "/Users/cpiquet/dev/esgi/sCrapper/sCrapper/resources/conf.sconf"
#endif /* main_h */
