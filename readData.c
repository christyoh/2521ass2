/* readData.c
 * Written by Victor Tsang (z5209633)
*/
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <assert.h>
#include "readData.h"

/* Create a set/list of urls to process by reading data from file
 * "collection.txt"
*/
Node getCollection() {
	FILE* urls = fopen("collection.txt", "r");
	char url[1000];
	
	Node head = NULL;
	while (fscanf(urls, "%s", url) != EOF) {
		char* input = malloc(sizeof(char)*(strlen(url)+1));
		strcpy(input, url);
		// 0 items
		if (head == NULL) {
			head = makeNode(input);
		}
		// multi items
		else {
			Node curr;
			for (curr = head; curr->next != NULL; curr = curr->next);
			Node new = makeNode(input);
			curr->next = new;
			curr = curr->next;
		}
	}
	fclose(urls);
	
	return head;	
}


/* Create empty graph (graph ADT from graph.h and graph.c)
 * Reads <url>.txt and updates graph by adding nodes and links
*/
Graph getGraph(Node urlList) {
	// get number of vertices
	int vNum = listLength(urlList);

	// create graph with that number of vertices
	Graph g = newGraph(vNum);

	// convert urlList to an array (for generating the graph)
	Vertex* vertices = calloc(vNum, sizeof(Vertex));
	int i = 0;
	Node curr;
	for (curr = urlList; curr != NULL; curr = curr->next) {
		vertices[i] = curr->v;		
		i++;
	}
	// fill graph adjacency list from the array of vertices
	fillGraph(g, vertices);
	
	for (i = 0; i < vNum; i++) {
		char* filename = malloc(sizeof(char)*(strlen(vertices[i]) + 4 + 1));
		strcpy(filename, vertices[i]);
		filename = strcat(filename, ".txt");
		
		FILE* fstream = fopen(filename, "r");
		
		bool startFlag = false;
		char str[1000] = {'\0'};
		while(fscanf(fstream, "%s", str) != EOF) {
			// if str is Section-1
			if (strcmp("Section-1", str) == 0) { //(isString("Section-1", str)) {
				startFlag = true;
			} else if (strcmp("#end", str) == 0) {
				startFlag = false;
				break;
			}
			else if (startFlag == true) {
				char* urlOut = malloc(sizeof(char)*(strlen(str)+1));
				strcpy(urlOut, str);
				// make an edge between vertices[i] and urlOut
				Edge newEdge = {.start = vertices[i], .end = urlOut};
				// insert edge into graph
				insertEdge(g, newEdge);
			}
		}	
		fclose(fstream);
		free(filename);
	}
	
	// remove self loops
	for (i = 0; i < vNum; i++) {
		if (adjacent(g, vertices[i], vertices[i])) {
			Edge self = {vertices[i], vertices[i]};
			removeEdge(g, self);
		}
	}
	// freeing vertices
	for (i = 0; i < vNum; i++) {
		free(vertices[i]);
	}
	free(vertices);
	return g;
}

// function to free collection
void freeCollection(Node head) {
	if (head != NULL) {
		freeCollection(head->next);
		free(head);
	}
}
