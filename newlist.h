// Linked List interface ... COMP2521 

#ifndef NEWLIST_H
#define NEWLIST_H

typedef struct Node *List;

typedef struct Url *UrlList;

UrlList makeNode(char*);
UrlList insertLL(UrlList, char*);
UrlList deleteLL(UrlList, char*);
void freeLL(UrlList);
void showLL(UrlList);

List makenode(int, char*);
List insertLL1(List, int, char*);
void insertURL(List, char*);
void showLL1(List);
void freeLL1(List);

#endif
