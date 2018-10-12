// Linked list implementation ... COMP2521 
// Taken from 2521 wk05 lecture (Ashesh)
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
typedef struct Node {
   char*		v;
   struct Node *next; 
} Node;

Node *makeNode(char* str) {
   Node *new = malloc(sizeof(Node));
   assert(new != NULL);
   new->v = str;
   new->next = NULL;
   return new;
}

List insertLL(List L, char* str) {
   if (inLL(L, str))
      return L;

   // add new node at the beginning
   Node *new = makeNode(str);
   new->next = L;
   return new;
}

List deleteLL(List L, char* str) {
   if (L == NULL)
      return L;
   if (strcmp(L->v, n) == 0)
      return L->next;

   L->next = deleteLL(L->next, n);
   return L;

}

bool inLL(List L, char* str) {
   if (L == NULL)
      return false;
   if (strcmp(L->v, n) == 0)
     return true;

   return inLL(L->next, str);
}

void showLL(List L) {
   if (L == NULL)
      putchar('\n');
   else {
      printf("%s ", L->v);
      showLL(L->next);
   }
}

void freeLL(List L) {
   if (L != NULL) {
      freeLL(L->next);
      free(L);
   }
}
