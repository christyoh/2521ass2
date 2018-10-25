// Set interface ... COMP2521

#ifndef SET_H
#define SET_H

#include <stdbool.h>

struct node {
	char* v;
	int* ranks;
	struct node *next; 
};

typedef struct node* Node;
typedef struct node* Set;

Node makeNode(char*, int);
void insertSet(Set, char*, int, int, int);
bool inSet(Set, char*);
Set  deleteFromSet(Set, char*);
void freeSet(Set);
void showSet(Set);
int  sizeSet(Set);
Node findNode(Set, char*);
char* findValue(Set, int);
int  sizeRF(Set, int);
int  posRF(Set, char*, int);
#endif
