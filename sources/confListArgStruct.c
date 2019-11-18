//
//  confListArgStruct.c
//  sCrapper
//
//  Created by CELIA PIQUET on 17/11/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//

#include "main.h"

char *getExtensionFromUrl(char *url){
    CURL *curl;
    char *ct = NULL;
    int result;
    if(url){
        curl = curl_easy_init();
        if (curl){
            curl_easy_setopt(curl, CURLOPT_URL, url);//Recuperation des informations au niv de l'URL
            curl_easy_setopt(curl, CURLOPT_FAILONERROR, 1L);//Gestion erreurs
            curl_easy_setopt(curl, CURLOPT_TIMEOUT, 20L);
            curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
            curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

            result = curl_easy_perform(curl);//Resultat telechargement
            
            if(result == CURLE_OK){
                result = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &ct);
            }
            curl_easy_cleanup(curl);
            
        }
    }
    return ct;
}

void cleanExtension(char *extension){
    if (extension != NULL && strcmp(extension,"(null)")){
        int len = (int)strlen(extension);
        char *extensionTmp = malloc(sizeof(char)*len);
        if(extensionTmp){
            int startExtension = 0, endExtension = len;
            for (int i = 0; i < len; i++){
                if (extension[i] == ';' && endExtension == len){
                    extension[i] = '\0';
                    endExtension = i;
                }
                if (extension[i] == '/' && startExtension == 0){
                    startExtension = i + 1;
                }
            }
            sprintf(extensionTmp, "%s", extension+startExtension);
            extension[0] = '\0';
            sprintf(extension, "%s", extensionTmp);
        }
    } else if (extension && (strcmp(extension, "(null)") == 0 || strlen(extension) == 0)) {
        sprintf(extension, "html");
    }
}

int loadArgsFromAction(Action *action){
    fprintf(stderr, "       Get args of action '%s'..\n", action->name);
    int i;
    if (action){
        ListArgStruct *args = initListArgStruct(action->allUrlsWithDepth.nbOfUrl);
        if (args){
            for (i = 0; i < action->allUrlsWithDepth.nbOfUrl; i++){
                ArgStruct *actualArgStruct = initArg(action->typesToTarget.nbOfType);
                if (actualArgStruct){
                    sprintf(actualArgStruct->actionName, "%s", action->name);
                    sprintf(actualArgStruct->url, "%s", action->allUrlsWithDepth.tabUrls[i]);
                    
                    actualArgStruct->index = i;
                    
                    actualArgStruct->needVersioning = action->hasVersionning;
                    
                    actualArgStruct->listType = action->typesToTarget;
                    
                    sprintf(actualArgStruct->extension, "%s", getExtensionFromUrl(actualArgStruct->url));
                    cleanExtension(actualArgStruct->extension);
                    
                    args->tabArg[i] = *actualArgStruct;
                    args->nbOfArgs ++;
                }
            }
            action->argsForScrapping = *args;
            return 1;
        }
    }
    return 0;
}

