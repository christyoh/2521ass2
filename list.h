// Linked list interface ... COMP2521 
#ifndef LIST_H
#define LIST_H

#include <stdbool.h>

struct listRep {
	char* key;
	double pagerank;
	struct _node *first;
};

struct _node {
	char*		v;
	struct _node *next; 
};

typedef struct listRep* List;
typedef struct _node* Node;

List newList(char*, Node);
Node makeNode(char* str);
List insertLL(List, char*);
bool inLL(Node, char*);
List deleteLL(List, char*);
void freeLL(Node);
void showLL(Node);
bool isKey(List, char*);
int  listLength(Node);

#endif
