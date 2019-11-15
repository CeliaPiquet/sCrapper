//
//  scrapping.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

void getNameOfFile(char *nameOfFile, char *url){
    int posOfFileName = 0;
    for (int i = 0; i < strlen(url); i++){
        if (url[i] == '/'){
            posOfFileName = i +1;
        }
    }
    strcpy(nameOfFile, url+posOfFileName);
}

static size_t write_data(void *ptr, size_t size, size_t nmemb, void *stream)
{
  size_t written = fwrite(ptr, size, nmemb, (FILE *)stream);
    return written;
}

static void* run_one_thread(void *url){
    CURL *curl_handle;
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
    
    // Set URL
    curl_easy_setopt(curl_handle, CURLOPT_URL, url);
    
    // Switch on full protocol/debug output while testing
    curl_easy_setopt(curl_handle, CURLOPT_VERBOSE, 1L);
    
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
    
    
     return NULL;
}

static void* startTaskTimer(void *task){
    Task *newTask = (Task*) task;
    // Lancer les threads par action et en fonction du timing 
    return NULL;
}

void startScrapping(ListTask *tasks){
    Task *actualTask = malloc(sizeof(Task));
    int i, error;
    pthread_t threads[tasks->nbOfTask];
    for (i = 0; i < tasks->nbOfTask; i++){
        actualTask = &tasks->tabTask[i];
        error = pthread_create(&threads[i], NULL, startTaskTimer, (void *)actualTask);
        if (error != 0){
            fprintf(stderr, "Couldn't run thread number %d, errno %d\n", i, error);
        } else {
            fprintf(stderr, "Thread %d, gets %s\n", i, actualTask->name);
        }
    }
    
    for(i = 0; i < tasks->nbOfTask; i++) {
      pthread_join(threads[i], NULL);
      fprintf(stderr, "Thread %d terminated\n", i);
    }

}
