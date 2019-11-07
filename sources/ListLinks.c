#include "../headers/scrapper.h"

struct ListLinks* newEmptyListOfLinks(){

	int initialCapacity = 100000;
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

	// if(ListOfLinks->size == ListOfLinks->capacity){

	// 	ListOfLinks->capacity *=2;

	// 	struct ListLinks* l2 = malloc(sizeof(ListLinks) * 1);

	// 	l2->links = realloc(ListOfLinks->links, sizeof(Link) * ListOfLinks->capacity);

	// 	ListOfLinks = l2;

	// 	// printf("%d -- %d", ListOfLinks->size, ListOfLinks->capacity);

	// 	// for(int i = ListOfLinks->size; i < ListOfLinks->capacity; i++)
	// 	// {
	// 	// 	ListOfLinks->links[i].href = malloc(sizeof(char) * 1000);
	// 	// }

	// 	// struct ListLinks* tempListOfLinks = malloc(sizeof(ListOfLinks));
	// 	// tempListOfLinks->links = realloc(ListOfLinks->links, (sizeof(Link) * ListOfLinks->capacity));

	// 	// printf("%s", tempListOfLinks->links[1].href);

	// 	// for(int i = 0; i < ListOfLinks->capacity; i++)
	// 	// {
	// 	// 	tempListOfLinks->links[i].href = malloc(sizeof(char) * 1000);
	// 	// }

	// 	// if (tempListOfLinks->links) {
	// 	//     ListOfLinks = tempListOfLinks;
	// 	// }else {
	// 	// 	printf("Erreur !");
	// 	// }
	// }

	strcpy(ListOfLinks->links[ListOfLinks->size].href, href);

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