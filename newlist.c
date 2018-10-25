// Linked List implementation ... COMP2521 
// Taken from 2521 wk05 lecture (Ashesh)

#include "newlist.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef struct Node {
   int           nterms;
   UrlList		 urls;
   struct Node  *next;
} Node;

typedef struct Url {
   char*	   v;
   struct Url *next; 
} Url;

UrlList makeNode(char* str) {
   Url *new = malloc(sizeof(Url));
   assert(new != NULL);
   new->v = malloc(sizeof(char) * strlen(str)+1);
   strcpy(new->v, str);
   new->next = NULL;
   return new;
}

UrlList insertLL(UrlList L, char* str) {
   // add new Node at the beginning
   Url *new = makeNode(str);
   new->next = L;
   return new;
}

UrlList deleteLL(UrlList L, char* str) {
   if (L == NULL)
      return L;
   if (strcmp(L->v, str) == 0)
      return L->next;

   L->next = deleteLL(L->next, str);
   return L;

}

void showLL(UrlList L) {
   if (L == NULL)
      putchar('\n');
   else {
      printf("%s ", L->v);
      showLL(L->next);
   }
}

void freeLL(UrlList L) {
   if (L != NULL) {
      freeLL(L->next);
      free(L->v);
      free(L);
   }
}

List makenode(int nterms, char* str) {
   Node *new = malloc(sizeof(Node));
   assert(new != NULL);
   new->nterms = nterms;
   new->urls = makeNode(str);
   new->next = NULL;
   return new;
   
}

List insertLL1(List L, int nterms, char* str) {
   
   // if List is empty, return new Node as head of List
   if (L == NULL) {
       Node *new = makenode(nterms, str);
       return new;
   }
    
   // insert url into already existing Node with correct no of terms
   List curr = L;
   while (curr != NULL) {
       if (curr->nterms == nterms) {
           insertURL(curr, str);
           return L;
       } else if (curr->nterms < nterms) {
           break;
       }
       curr = curr->next;
   }
   
   // prepend Node containing url to L
   Node *new = makenode(nterms, str);

   curr = L;
   if (curr->nterms < nterms) {
       new->next = L;
       return new;
   }
   
   // add new Node in order
   while (curr->next != NULL) {
       if (curr->next->nterms < nterms) {
           new->next = curr->next;
           curr->next = new;
           return L;
       }
       curr = curr->next;
   }
   // append new Node to L
   curr->next = new;
   
   return L;
}

void insertURL(List Node, char* url) {
   
   UrlList curr = Node->urls;
   if (curr == NULL) {
       curr = makeNode(url);
   } else {
       while (curr->next != NULL) {
           if (strcmp(curr->v, url) == 0) {
               break;
           }
           curr = curr->next;
       }
       if (strcmp(curr->v, url) != 0) {
           curr->next = makeNode(url);
       }
   }
   
}

void showLL1(List L) {
   
   List curr = L;
   UrlList temp;
   while (curr != NULL) {
       temp = curr->urls;
       while (temp != NULL) {
           printf("%s ", temp->v);
           temp = temp->next;
       }
       curr = curr->next;
   }
   printf("\n");
   
}

void freeLL1(List L) {
   
   if (L != NULL) {
      freeLL1(L->next);
      freeLL(L->urls);
      free(L);
   }
   
}
