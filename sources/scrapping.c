//
//  scrapping.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"



void mkdirRecursive(char *path){
    int sizeToCopy = 0, isDirectory = 0, sizePath, i;
    char *pathCopy = malloc(sizeof(char)*strlen(path));
    if(pathCopy){
        sizePath = (int)strlen(path);
        while (sizeToCopy < sizePath-1){
            isDirectory = 0;
            for (i = sizeToCopy+1; i <= sizePath; i++){
                if (path[i] == '/'){
                    sizeToCopy = i;
                    isDirectory = 1;
                    break;
                }
            }
            if (!isDirectory){ // il nous reste alors que le nom du fichier
                return;
            }
            strncpy(pathCopy, path, sizeToCopy);
            pathCopy[sizeToCopy] = '\0';
            if(mkdir(pathCopy,0777)==0){
                fprintf(stderr, "%s was created\n", pathCopy);
            }
           
        }
        free(pathCopy);
    }
    
}

void *downloadOneUrl(void *arguments){
    if (arguments){
        struct ArgStruct *args = (struct ArgStruct*) arguments;
        char *pathFile = malloc(sizeof(char)*SIZE_MAX_URL);
        if(pathFile){
            if (args->needVersioning){
                time_t rawtime;
                struct tm * timeinfo;
                time ( &rawtime );
                timeinfo = localtime ( &rawtime );
                sprintf(pathFile, "%sdownloads/%s/version_[%d-%d-%d %dh%dmin%ds]/file%d.html", PARENT_PATH, args->actionName,timeinfo->tm_mday, timeinfo->tm_mon + 1, timeinfo->tm_year + 1900, timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, args->index);
            } else {
                sprintf(pathFile, "%sdownloads/%s/file%d.html", PARENT_PATH, args->actionName, args->index);
            }
            mkdirRecursive(pathFile);
            CURL* curl = NULL;
            FILE* fp = fopen(pathFile,"wb+");
            int result;

            if (fp == NULL){
                printf("Erreur d'ouverture de fichier\n");
            
            } else {
    
                curl = curl_easy_init();//Initialisation
                 
                if (curl){
                    curl_easy_setopt(curl, CURLOPT_URL, args->url);//Recuperation des informations au niv de l'URL
                    curl_easy_setopt(curl, CURLOPT_WRITEDATA, fp);//Ecriture dans le fichier
                    curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs
                    curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
                    curl_easy_setopt(curl, CURLOPT_NOSIGNAL, 1L);

                    result = curl_easy_perform(curl);//Resultat telechargement

                    if (result != CURLE_OK){
                        fprintf(stderr,"    CURL ERROR: %s\n", curl_easy_strerror(result));
                    } else {
//                        if(!checkContentType(curl, args->listType)){
//                            //remove(pathFile);
//                        }
                    }

                    curl_easy_cleanup(curl);//Vide les ressources de curl
                }
                fclose(fp);
            }
             
            free(pathFile);
        }
    }
    pthread_exit(NULL);
    return NULL;
}



void setArgUrl(ArgStruct *arg, char *url){
    int lenUrl = (int)strlen(url);
    strcpy(arg->url, url);
    if (lenUrl < SIZE_MAX_URL){
        arg->url[lenUrl] = '\0';
    }
}

void setArgActionName(ArgStruct *arg, char *name){
    int lenName = (int)strlen(name);
    strcpy(arg->actionName, name);
    if (lenName < SIZE_MAX_URL){
        arg->actionName[lenName] = '\0';
    }
}


//void freeArg(ArgStruct *arg){
//    if (arg){
//        if (arg->actionName){
//            free(arg->actionName);
//        }
//        if (arg->url){
//            free(arg->url);
//        }
//        free(arg);
//    }
//}
//
//void freeListArg(ListArgStruct *args){
//    if (args){
//        int i;
//        for (i = 0; i < args->nbOfArgs; i++){
//            freeArg(&(args->tabArg[i]));
//        }
//        free(args);
//    }
//}


void scrappOneAction(Action *action){
    if(action){
        
        int i, nbThreads = action->allUrlsWithDepth.nbOfUrl;
        ListArgStruct *args = initListArgStruct(nbThreads);
        
        
        if (args){
            pthread_t tid[nbThreads];
            for (i = 0; i < nbThreads; i++){
                ArgStruct actualArg = *initArg(action->typesToTarget.nbOfType);
                setArgUrl(&actualArg, action->allUrlsWithDepth.tabUrls[i]);
                setArgActionName(&actualArg, action->name);
                actualArg.needVersioning = action->hasVersionning;
                actualArg.index = i;
                actualArg.listType = action->typesToTarget;
                args->tabArg[args->nbOfArgs] = actualArg;
                args->nbOfArgs ++;
                
                if(args->tabArg[i].actionName && args->tabArg[i].url){
                    pthread_create(&tid[i], NULL, downloadOneUrl, (void *)&(args->tabArg[i]));
                }
            }
            for (i = 0; i <nbThreads; i++){
                pthread_join(tid[i], NULL);
            }
            //freeListArg(args);
            free(args);
        }
    }
    
}

void *scrappOneTask(void* task){
    Task *actualTask = (Task *)task;
    while (1){
        fprintf(stderr,"-> Task %s running ..\n", actualTask->name);
        for (int j = 0; j < actualTask->actionsToRun.nbOfAction; j++){
            scrappOneAction(&(actualTask->actionsToRun.tabAction[j]));
        }
        fprintf(stderr,"--> Task %s completed !\n", actualTask->name);
        fprintf(stderr,"---> Time before next running : %d h %d min %d s\n", actualTask->hour, actualTask->minute, actualTask->second);
        sleep(actualTask->second + (actualTask->minute)*60 + (actualTask->hour)*3600);
    }
    return NULL;
}

void startScrapping(ListTask *tasks){
    int i, nbTasks = tasks->nbOfTask;
    pthread_t threads[nbTasks];
    for(i = 0; i < nbTasks; i++){
        pthread_create(&threads[i], NULL, scrappOneTask, &(tasks->tabTask[i]));
    }
    for (i = 0; i < nbTasks; i++){
        pthread_join(threads[i], NULL);
    }
    
}
