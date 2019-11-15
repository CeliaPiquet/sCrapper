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

    #define SIZE_MAX_ATTRIBUT_NAME  30
    #define SIZE_MAX_ATTRIBUT_VALUE  1000
    #define SIZE_MAX_OF_LINE_CONF 1000
    #define SIZE_MAX_OF_LINE_DOWNLOADS  2000
    #define PARENT_PATH "/Users/cpiquet/dev/esgi/sCrapper/sCrapper/"
#endif /* main_h */
