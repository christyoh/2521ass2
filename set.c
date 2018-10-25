// Linked list implementation ... COMP2521 
// Taken from 2521 wk05 lecture (Ashesh)
#include "set.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdbool.h>

Node makeNode(char* str, int nFiles) {
	// malloc the input string
	char* input = malloc(sizeof(char)*(strlen(str) + 1));
	strcpy(input, str);
	
	Node new = malloc(sizeof(struct node));
	assert(new != NULL);
	new->v = input;
	new->ranks = calloc(nFiles, sizeof(int));
	new->next = NULL;
	return new;
}

void insertSet(Set S, char* str, int fileIndex, int rank, int nFiles) {
	// if already in
	// don't add, but just update the ranks array for that node
	if (inSet(S, str)) {
		Node curr = S;
		while (curr != NULL && strcmp(curr->v, str) != 0) curr = curr->next;
		curr->ranks[fileIndex] = rank;
		return;
	}
	// add new node at the end
	Node new = makeNode(str, nFiles);
	if (new->ranks[fileIndex] == 0) new->ranks[fileIndex] = rank;
	// case: empty list
	if (S == NULL) {
		S = new;
		return;
	} else {
		Node curr;
		for (curr = S; curr->next != NULL; curr = curr->next);
		curr->next = new;
	}
}

// returns true or false
bool inSet(Set S, char* str) {
	if (S == NULL)
	  return false;
	if (strcmp(S->v, str) == 0)
	 return true;

	return inSet(S->next, str);
}

void showSet(Set S) {
	if (S == NULL) {
		putchar('\n');
	}
	else {
	  printf("(%d. %s | %d. %s)", S->ranks[0], S->v, S->ranks[1], S->v);
	  if (S->next != NULL) printf("\n");
	  showSet(S->next);
	}
}
// frees a specific node in a linked list
Set deleteFromSet(Set S, char* str) {
	if (S == NULL) return S;
	
	Node temp = S;
	// str is the first elem of set
	if (strcmp(S->v, str) == 0) {
		S = S->next;
		free(temp);
	} // else
	else {
		Node prev = S;
		Node curr = S->next;
		while (curr != NULL) {
			if (strcmp(curr->v, str) == 0) {
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
	
	return S;
}
// frees a set starting from the head
void freeSet(Set S) {
	if (S != NULL) {
		freeSet(S->next);
		free(S->v);
		free(S->ranks);
		free(S);
	}	
}

int sizeSet(Set S) {
	int length = 0;
	if (S != NULL) {
		Node curr;
		for (curr = S; curr != NULL; curr = curr->next) length++;
	}
	return length;
}

// find the node storing this string
Node findNode(Set S, char* str) {
	assert(S != NULL);
	Node curr = S;
	while (curr != NULL && strcmp(curr->v, str) != 0) curr = curr->next;
	return curr;
}

// find the value of a node with position pos
char* findValue(Set S, int pos) {
	assert(S != NULL);
	assert(pos != 0);
	int count = 1;
	Node curr = S;
	while (curr != NULL && count != pos) {
		curr = curr->next;
		count += 1;
	}
	return curr->v;
}

// counts the size of a ranklist give its index in ranks array
int sizeRF(Set S, int fileIndex) {
	assert(S != NULL);
	int count = 0;
	Node curr;
	for (curr = S; curr != NULL; curr = curr->next) {
		if (curr->ranks[fileIndex] != 0) {
			count++;
		}
	}
	return count;
}
// finds the position of an element elem in a rankFile
int posRF(Set S, char* elem, int fileIndex) {
	Node found = findNode(S, elem);
	if (found->ranks[fileIndex] != 0) return found->ranks[fileIndex];
	else return 0;
}
