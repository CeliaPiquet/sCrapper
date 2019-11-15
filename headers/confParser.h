//
//  confParser.h
//  sCrapper
//
//  Created by CELIA PIQUET on 12/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#ifndef confParser_h
#define confParser_h

    ListTask *readConf(char *confFilePath);
int getOneLine(char* lineToUpdate, FILE *fp, int *charReaden, int sizeOfLine);
    void cleanOneLine(char *line);
    void cleanLineUntilSpecificChar(char *line, char stop, int hasToStopNext);
    int isCorrectAttributName(char *attributName);
    int getAttributName(char *attributName, char *line);
    void getAttributValueStr(char *actualLine, char *actualAttributValue, int index, int sizeActualAttributValue);
    int getAttributValueTabStr(char *actualLine, char **attributValue, int index);

#endif /* confParser_h */
