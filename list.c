// Linked list implementation ... COMP2521 
// Taken from 2521 wk05 lecture (Ashesh)
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

List newList(char* key, Node head) {
	List newL = malloc(sizeof(struct listRep));
	newL->key = key;
	newL->pagerank = 0;
	newL->first = head;
	return newL;
}

Node makeNode(char* str) {
	Node new = malloc(sizeof(struct _node));
	assert(new != NULL);
	new->v = str;
	new->next = NULL;
	return new;
}

List insertLL(List L, char* str) {
	if (inLL(L->first, str))
	  return L;

	// add new node at the end
	Node new = makeNode(str);
	
	// case: empty list
	if (L->first == NULL) L->first = new;
	else {
		Node curr;
		for (curr = L->first; curr->next != NULL; curr = curr->next);
		curr->next = new;
	}
	return L;
}

bool inLL(Node head, char* str) {
	if (head == NULL)
	  return false;
	if (strcmp(head->v, str) == 0)
	 return true;

	return inLL(head->next, str);
}

void showLL(Node head) {
	if (head == NULL) {
		putchar('\n');
	}
	else {
	  printf("%s", head->v);
	  if (head->next != NULL) printf(" -> ");
	  showLL(head->next);
	}
}

List deleteLL(List L, char* v) {
	if (L == NULL) return L;
	Node temp = L->first;
	if (strcmp(L->first->v, v) == 0) {
		L->first = L->first->next;
		free(temp);
	}
	else {
		Node prev = L->first;
		Node curr = L->first->next;
		while (curr != NULL) {
			if (strcmp(curr->v, v) == 0) {
				temp = curr;
				prev->next = curr->next;
				curr = curr->next;
				free(temp);
			}
			else {
				prev = curr;
				curr = curr->next;
			}
		}
	}
	
	return L;
}

void freeLL(Node head) {
	if (head != NULL) {
	  freeLL(head->next);
	  free(head->v);
	  free(head);
	}
}

bool isKey(List L, char* str) {
	if (L == NULL) return false;
	if (strcmp(L->key, str) == 0) return true;
	else return false;
}

int listLength(Node head) {
	int length = 0;
	if (head != NULL) {
		Node curr;
		for (curr = head; curr != NULL; curr = curr->next) length++;
	}
	return length;
}
