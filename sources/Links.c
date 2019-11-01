#include "../headers/scrapper.h"

struct Link* newEmptyLink(){

	int initialCapacity = 1000;
	struct Link* link = malloc(sizeof(Link));

	link->href = malloc(sizeof(char) * initialCapacity);
	link->depth = 0;
	link->stringSize = 0;

	return link;
}