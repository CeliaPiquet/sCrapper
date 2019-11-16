//
//  scrapping.h
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef scrapping_h
#define scrapping_h

void startScrapping(ListTask *tasks);
void getNameOfFile(char *nameOfFile, char *url);
int checkContentType(CURL* curl, char* typeAction);

#endif /* scrapping_h */
