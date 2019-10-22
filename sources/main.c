//
//  main.c
//  sCrapper
//
//  Created by CELIA PIQUET on 14/10/2019.
//  Copyright © 2019 CELIA PIQUET. All rights reserved.
//
#include "../Headers/scrapper.h"

int main(int argc, const char * argv[]) {
//    CURL *curlHandler;
//    FILE *fp = fopen("/Users/cpiquet/dev/sCrapper/sCrapper/downloads/home.html","w+"); //fichier qui va recevoir l'information téléchargée
//    int result;
//
//    if (fp == NULL){
//        printf("Erreur d'ouverture de fichier\n");
//        return 1;
//    }
//
//    curlHandler = curl_easy_init();
//    curl_easy_setopt(curlHandler, CURLOPT_URL, "https://stackoverflow.com/questions/5793760/including-libcurl-in-c-project");
//    curl_easy_setopt(curlHandler, CURLOPT_WRITEDATA, fp);
//    curl_easy_setopt(curlHandler, CURLOPT_FAILONERROR, 1L);
//
//    result = curl_easy_perform(curlHandler);
//
//    if(result == CURLE_OK){
//        printf("Download successful !\n");
//    } else {
//        printf("ERROR: %s\n", curl_easy_strerror(result));
//    }
//    fclose(fp);
    
    readConf("/Users/cpiquet/dev/sCrapper/sCrapper/ressources/conf.sconf");
    
    return 0;
}
