//
//  curlFunctions.h
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef curlFunctions_h
#define curlFunctions_h

void getLinks(FILE *fp, Action *action);
void getHtmlPage(char* savePath, char* url);
void getAllLinks(struct ListLinks* ListLinks, int depth);

#endif /* curlFunctions_h */
