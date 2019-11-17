//
//  confListLinks.h
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confListLinks_h
#define confListLinks_h

void displayListLinks(ListLinks list);
int isAlreadyInList(ListLinks *list, char *url);
int addLinkToList(ListLinks *list, char *url);
void freeListLinks(ListLinks *list);
ListLinks* initListLinks(int capacity);
int completeListLinks(Action *action);


#endif /* confListLinks_h */
