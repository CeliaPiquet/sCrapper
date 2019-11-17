//
//  confListArgStruc.h
//  sCrapper
//
//  Created by CELIA PIQUET on 17/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confListArgStruc_h
#define confListArgStruc_h
ListArgStruct* initListArgStruct(int capacity);
ArgStruct* initArg(int nbTypes);

char *getExtensionFromUrl(char *url);
void cleanExtension(char *extension);
int loadArgsFromAction(Action *action);


#endif /* confListArgStruc_h */
