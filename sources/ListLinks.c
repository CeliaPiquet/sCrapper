#include "../headers/scrapper.h"

struct ListLinks* newEmptyListOfLinks(){

	int initialCapacity = 300;
	int initialCapacityLink = 1000;

	struct ListLinks* ListOfLinks = malloc(sizeof(ListOfLinks));

	ListOfLinks->links = malloc(sizeof(Link) * initialCapacity);

	for(int i = 0; i < initialCapacity; i++)
	{
		ListOfLinks->links[i].href = malloc(sizeof(char) * initialCapacityLink);
	}

	ListOfLinks->capacity = initialCapacity;
	ListOfLinks->size = 0;

	return ListOfLinks;
}

void ListOfLinksAdd(struct ListLinks* ListOfLinks, char* href, int depth){

	strcpy(ListOfLinks->links[ListOfLinks->size].href, href);

	//ListOfLinks->links[ListOfLinks->size].href = href;
	ListOfLinks->links[ListOfLinks->size].depth = depth;
	ListOfLinks->links[ListOfLinks->size].stringSize = strlen(href);

	ListOfLinks->size++;
}

struct Link ListOfLinksGetLink(struct ListLinks* ListOfLinks, int index){
	return ListOfLinks->links[index];
}

void ListOfLinksToStringDebug(struct ListLinks* ListOfLinks){

	for(int i = 0; i < ListOfLinks->size; i++)
	{
		printf("Lien n°%d : %s | depth : %d | Taille : %lu \n", i+1, ListOfLinks->links[i].href, ListOfLinks->links[i].depth , ListOfLinks->links[i].stringSize);
	}

}