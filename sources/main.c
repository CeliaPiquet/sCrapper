//
//  main.c
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "../headers/main.h"

int main(int argc, const char * argv[]) {
    fprintf(stderr,"Loading configuration...\n\n");
    ListTask *tasksToRun = NULL;

    curl_global_init(CURL_GLOBAL_ALL);
    tasksToRun = readConf(CONF_PATH);


    if (tasksToRun != NULL){
        fprintf(stderr,"\nConfiguration ok\n\nRunning tasks :\n");
        for (int i = 0; i < tasksToRun->nbOfTask; i++){
            displayTask(tasksToRun->tabTask[i]);
        }
        
        startScrapping(tasksToRun);
        return 1;
    }
    curl_global_cleanup();
    return 0;
}
