//
//  scrapping.h
//  sCrapper
//
//  Created by CELIA PIQUET on 06/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef scrapping_h
#define scrapping_h

    #include <stdio.h>
    #include "confTask.h"
    #include "scrapper.h"

    int startScrapping(ListTask tasks);
    void *writeOnFile(void* url);
    void mkdirRecursive(char *path);
    void initListThreads(ListThread *listThreads);

#endif /* scrapping_h */
