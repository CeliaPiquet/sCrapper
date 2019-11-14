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
    
    char *confFilePath = "/Users/cpiquet/dev/projets_esgi/sCrapper/sCrapper/resources/conf.sconf";
    
    curl_global_init(CURL_GLOBAL_ALL);
    tasksToRun = readConf(confFilePath);
    
    
    if (tasksToRun != NULL){
        // Scrapping
        for (int i = 0; i < tasksToRun->nbOfTask; i++){
            displayTask(tasksToRun->tabTask[i]);
        }
        startScrapping(tasksToRun);
        return 1;
    }
    curl_global_cleanup();
    return 0;
}
