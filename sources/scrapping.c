//
//  scrapping.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

void getNameOfFile(char *nameOfFile, char *url){
    int sizeOfUrl = (int)strlen(url);
    int posStartName = 0, sizeToCopy, posEndName = sizeOfUrl;
    for (int i = 0; i < sizeOfUrl; i++){
        if (url[i] == '/'){
            posStartName = i + 1;
            continue;
        }
        if(url[i] == '?' || (url[i] == '#' && url[i-1] != '/')){
            posEndName = i;
            break;
        }
    }
    sizeToCopy = posEndName - posStartName;
    strncpy(nameOfFile, url+posStartName, sizeToCopy);
    if (sizeToCopy < 200){
        nameOfFile[sizeToCopy] = '\0';
    }
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

static void* run_one_thread(void *url){
    CURL *curl_handle = NULL;
    char *pagefilename = malloc(sizeof(char)*200);
    char *nameOfFile = malloc(sizeof(char)*20);
    if (pagefilename == NULL || nameOfFile == NULL){
        return NULL;
    }
    pagefilename[0]= '\0';
    getNameOfFile(nameOfFile, url);
    sprintf(pagefilename, "%sdownloads/1%s.html", PARENT_PATH, nameOfFile);
    FILE *pagefile;
    
    
    // init the CURL session
    curl_handle = curl_easy_init();
    
    if (curl_handle){
         curl_easy_setopt(curl_handle, CURLOPT_URL, url); // Set URL
         //curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 20L); // timeout de 20 secondes
         curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L); // Switch on full protocol/debug output while testing
         
         /* disable progress meter, set to 0L to enable and disable debug output */
         curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
         
         /* send all data to this function  */
         curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);

         /* open the file */
         pagefile = fopen(pagefilename, "w+");
        
         if(pagefile) {
             
             /* write the page body to this file handle */
             curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);

            /* get it! */
            curl_easy_perform(curl_handle);

            /* close the header file */
            fclose(pagefile);
         } else {
             printf("page file NOT ok\n");
         }
         /* cleanup curl stuff */
          curl_easy_cleanup(curl_handle);
    }
    
    
     return NULL;
}

void* startUrlThreads(void *url){
    if (url){
        char *nameOfFile = malloc(sizeof(char)*20);
        char *pagefilename = malloc(sizeof(char)*SIZE_MAX_URL);
        if (nameOfFile){
            if (pagefilename){
                getNameOfFile(nameOfFile, url);
                sprintf(pagefilename, "%sdownloads/%s.html", PARENT_PATH, nameOfFile);
                if(strlen(pagefilename)<SIZE_MAX_URL){
                    pagefilename[strlen(pagefilename)] = '\0';
                }
                fprintf(stderr, "-> %s\n", pagefilename);
                
                CURL *curl_handle = curl_easy_init();
    
                if (curl_handle){
                     curl_easy_setopt(curl_handle, CURLOPT_URL, url); // Set URL
                     //curl_easy_setopt(curl_handle, CURLOPT_TIMEOUT, 20L); // timeout de 20 secondes
                     curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L); // Switch on full protocol/debug output while testing
    
                     /* disable progress meter, set to 0L to enable and disable debug output */
                     curl_easy_setopt(curl_handle, CURLOPT_NOPROGRESS, 1L);
    
                     /* send all data to this function  */
                     curl_easy_setopt(curl_handle, CURLOPT_WRITEFUNCTION, write_data);
    
                     /* open the file */
                     FILE *pagefile = fopen(pagefilename, "w+");
    
                     if(pagefile) {
    
                         /* write the page body to this file handle */
                         curl_easy_setopt(curl_handle, CURLOPT_WRITEDATA, pagefile);
                        
                        /* get it! */
                        curl_easy_perform(curl_handle);
    
                        /* close the header file */
                        fclose(pagefile);
                     } else {
                         printf("page file NOT ok\n");
                     }
                     /* cleanup curl stuff */
                      curl_easy_cleanup(curl_handle);
                }
                free(pagefilename);
            }
            free(nameOfFile);
        }
    }
    return NULL;
}

static void* startActionThreads(void *action){
    Action *newAction = (Action *) action;
    if (!newAction){
        return NULL;
    }
    int i, error;
    pthread_t threads[newAction->allUrlsWithDepth.nbOfUrl];
    for (i = 0; i < newAction->allUrlsWithDepth.nbOfUrl; i++){
        error = pthread_create(&threads[i], NULL, startUrlThreads, newAction->allUrlsWithDepth.tabUrls[i]);
        if (error != 0){
            fprintf(stderr, "Couldn't run task thread number 2-%d, errno %d\n", i, error);
        } else {
            fprintf(stderr, "Thread %d, gets %s\n", i, newAction->allUrlsWithDepth.tabUrls[i]);
        }
    }
    for(i = 0; i <  newAction->allUrlsWithDepth.nbOfUrl; i++) {
      pthread_join(threads[i], NULL);
      fprintf(stderr, "Thread 2-%d terminated\n", i);
    }
    return NULL;
}

static void* startTaskTimer(void *task){
    Task *newTask = (Task*) task;
    if (!newTask){
        return NULL;
    }
    // Lancer les threads par action et en fonction du timing
    int i, error;
    pthread_t threads[newTask->actionsToRun.nbOfAction];
    for (i = 0; i < newTask->actionsToRun.nbOfAction; i++){
        error = pthread_create(&threads[i], NULL, startActionThreads, &(newTask->actionsToRun.tabAction[i]));
        if (error != 0){
            fprintf(stderr, "Couldn't run task thread number 1-%d, errno %d\n", i, error);
        } else {
            fprintf(stderr, "Thread %d, gets %s\n", i, newTask->actionsToRun.tabAction[i].name);
        }
    }
    for(i = 0; i <  newTask->actionsToRun.nbOfAction; i++) {
      pthread_join(threads[i], NULL);
      fprintf(stderr, "Thread 1-%d terminated\n", i);
    }
    return NULL;
}

void startScrapping(ListTask *tasks){
    int i, error;
    pthread_t threads[tasks->nbOfTask];
    for (i = 0; i < tasks->nbOfTask; i++){
        error = pthread_create(&threads[i], NULL, startTaskTimer, (void *)&(tasks->tabTask[i]));
        if (error != 0){
            fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
        } else {
            fprintf(stderr, "Thread %d, gets %s\n", i, tasks->tabTask[i].name);
        }
    }
    
    for(i = 0; i < tasks->nbOfTask; i++) {
      pthread_join(threads[i], NULL);
      fprintf(stderr, "Thread 0-%d terminated\n", i);
    }

}

int checkContentType(CURL* curl, char* typeAction){
    int result;
    char* typeContentCurl;

    if(curl)
    {
        curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &typeContentCurl);

        result = strcmp(typeContentCurl, typeAction);

        if(result == 0){
            printf("Content type authorized");
            return 1;
        }
        else{
            printf("The content type doesn't match.");
            return 0;
        }
    }
    else
    {
        printf("Curl object null");
        return 0;
    }
}