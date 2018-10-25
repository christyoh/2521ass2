#ifndef SEARCHPAGERANK_H
#define SEARCHPAGERANK_H

typedef struct Node *List;

typedef struct Url *UrlList;

// Function Signatures

int searchPagerank(int argc, char *argv);
UrlList findMatchedUrls(int, char **);
List createList(UrlList);
void orderURLs(List);
int findPagerank(UrlList, int);
void argCheck(int);
void freeQuery(char **, int);

#endif
