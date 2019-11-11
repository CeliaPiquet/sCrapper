//
//  urlParser.h
//  sCrapper
//
//  Created by CELIA PIQUET on 10/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef urlParser_h
#define urlParser_h

#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>
#include "scrapper.h"
#include "types.h"


void ListOfLinksToStringDebug(struct ListLinks* ListOfLinks);

void ListOfLinksAdd(struct ListLinks* ListOfLinks, char* href, int depth);

void getLinks(FILE *fp, struct ListLinks* ListLinks);

void getLinksTillDepth(char* FileName, struct ListLinks* ListLinks, int depth);

void getPage(char* savePath, char* url);

void getAllLinks(struct ListLinks* ListLinks, int depth);

void getBalises(FILE *fp, struct ListLinks* ListLinks);

// char* isImage(char* link);

// void downloadType(char* link, int numDownload);

// void downloadDoc(char* link, int numDownload, char* extension, char* file);

#endif /* urlParser_h */
