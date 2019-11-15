//
//  main.c
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

int main(int argc, const char * argv[]) {
    ListTask *tasksToRun = NULL;

    curl_global_init(CURL_GLOBAL_ALL);
    tasksToRun = readConf(CONF_PATH);


    if (tasksToRun != NULL){
        // Scrapping
        for (int i = 0; i < tasksToRun->nbOfTask; i++){
            displayTask(tasksToRun->tabTask[i]);
        }
        startScrapping(tasksToRun);
        return 1;
    }
    curl_global_cleanup();
//    char *nameOfFile = malloc(sizeof(char)*200);
//    if (nameOfFile){
//        getNameOfFile(nameOfFile, "https://cdn.sstatic.net/Sites/stackoverflow/img/favicon.ico?v=4f32ecc8f43d");
//        printf("%s\n", nameOfFile);
//    }
    return 0;
}
