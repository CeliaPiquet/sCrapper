#include <stdio.h>
#include <stdlib.h>
#include <curl/curl.h>
#include <string.h>

struct Link Link;
struct ListLinks{
	struct Link* links;
	int capacity;
	int size;
};

struct ListLinks ListLinks;
struct Link{
	char* href;
	int depth;
	unsigned long stringSize;
};

struct ListLinks* newEmptyListOfLinks();

struct Link* newEmptyLink();

struct Link ListOfLinksGetLink(struct ListLinks* ListOfLinks, int index);

void ListOfLinksToStringDebug(struct ListLinks* ListOfLinks);

void ListOfLinksAdd(struct ListLinks* ListOfLinks, char* href, int depth);

void getLinks(FILE *fp, struct ListLinks* ListLinks);

void getPage(char* savePath, char* url);

// char* isImage(char* link);

// void downloadType(char* link, int numDownload);

// void downloadDoc(char* link, int numDownload, char* extension, char* file);