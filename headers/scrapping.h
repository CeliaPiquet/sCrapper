//
//  scrapping.h
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef scrapping_h
#define scrapping_h

void mkdirRecursive(char *path);
void *downloadOneUrl(void *arguments);
void setArgUrl(ArgStruct *arg, char *url);
void setArgActionName(ArgStruct *arg, char *name);
void scrappOneAction(Action *action);
void *scrappOneTask(void* task);
void startScrapping(ListTask *tasks);


#endif /* scrapping_h */
