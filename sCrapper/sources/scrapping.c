//
//  scrapping.c
//  sCrapper
//
//  Created by CELIA PIQUET on 06/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "scrapping.h"

void mkdirRecursive(char *path){
    int sizeToCopy = 0, isDirectory = 0;
    char *pathCopy = malloc(sizeof(char)*strlen(path));
    while (sizeToCopy < strlen(path)-1){
        isDirectory = 0;
        for (int i=sizeToCopy+1; i<=strlen(path); i++){
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
    
}


void *writeOnFile(void* url){
    CURL *curlHandler;
    char *destPath = malloc(sizeof(char)*2000);
    strcpy(destPath, "downloads/");
    strcat(destPath, url+8);
    strcat(destPath, ".html");
    char *filePath = malloc(sizeof(char)*(strlen(PARENT_PATH)+strlen(destPath)));
    strcpy(filePath, PARENT_PATH);
    strcat(filePath, destPath);
    mkdirRecursive(filePath);
    FILE *fp = fopen(filePath,"w+"); //fichier qui va recevoir l'information téléchargée
    int result;

    if (fp == NULL){
        fprintf(stderr,"Erreur d'ouverture de fichier\n");
        return NULL;
    }

    curlHandler = curl_easy_init();
    curl_easy_setopt(curlHandler, CURLOPT_URL, url);
    curl_easy_setopt(curlHandler, CURLOPT_WRITEDATA, fp);
    curl_easy_setopt(curlHandler, CURLOPT_FAILONERROR, 1L);

    result = curl_easy_perform(curlHandler);
    curl_easy_cleanup(curlHandler);

    if(result == CURLE_OK){
        fprintf(stderr, "Download successful !\n");
    } else {
        fprintf(stderr, "ERROR: %s\n", curl_easy_strerror(result));
    }
    fclose(fp);
    free(destPath);
    destPath = NULL;
    free(filePath);
    filePath = NULL;
    return NULL;
}

void createActionThreads(ListAction *actionList, ListThread *threadsList){
    int error;
    pthread_t threads[actionList->nbAction];
    for (int i=0; i<actionList->nbAction; i++){
        error = pthread_create(&threads[i], NULL, writeOnFile,actionList->actions[i].url);
        if (error != 0){
            fprintf(stderr, "Couldn't run thread number %d, errno %d,\n", i, error);
        }
    }
    for (int i=0; i<actionList->nbAction; i++){
        if (i < threadsList->sizeMax){
            threadsList->threads[threadsList->nbThreads] = threads[i];
            threadsList->nbThreads ++;
        }
    }
}

void initListThreads(ListThread *listThreads){
    listThreads->nbThreads = 0;
    listThreads->sizeMax = 10;
    listThreads->threads = malloc(sizeof(pthread_t)*listThreads->sizeMax);
}

void runThreads(Task task, ListThread listThreads){
    unsigned int timeToWait = task.second + task.minute*60 + task.hour*3600;
    while (1){
        for (int i=0; i < listThreads.nbThreads; i++){
            pthread_join(listThreads.threads[i], NULL);
            fprintf(stderr, "Thread %d terminated\n", i);
        }
        printf("time to wait : %d\n", timeToWait);
        for (int i=0; i<timeToWait; i++){
            sleep(1);
            printf("%d ",i);
        }
        printf("\n");
    }
}

int startScrapping(ListTask tasks){                 //à actualiser avec la liste des urls
    Task actualTask;
    int nbThread;
    curl_global_init(CURL_GLOBAL_ALL); // initialize libcurl
    
    for (int j = 0; j < tasks.nbTask; j++) {
        ListThread listThreads;
        initListThreads(&listThreads);
        actualTask = tasks.tasks[j];
        nbThread = actualTask.actionsList.nbAction;
        createActionThreads(&actualTask.actionsList, &listThreads);
        runThreads(actualTask, listThreads);
    }
    
  
    
    
    return 0; //No error
}


