//
//  confListLinks.h
//  sCrapper
//
//  Created by CELIA PIQUET on 14/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confListLinks_h
#define confListLinks_h

    ListLinks* initListLinks(int capacity);
int addLinkToList(ListLinks *list, char *url);
    void freeListLinks(ListLinks *list);
    int completeListLinks(Action *action);
int isAlreadyInList(ListLinks *list, char *url);
void displayListLinks(ListLinks list);

#endif /* confListLinks_h */
