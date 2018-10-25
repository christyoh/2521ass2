// Altered version of Graph.c from 2521 wk05 lecture
// Altered by Victor Tsang (z5209633)
// Date: 11 Oct 2018

#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

#include "Graph.h"

typedef struct GraphRep {
   List* edges;  // adjacency list
   int   nV;     // #vertices
   int   nE;     // #edges
} GraphRep;

Graph newGraph(int nV) {
	assert(nV >= 0);
	int i;

	Graph g = malloc(sizeof(GraphRep));
	assert(g != NULL);
	g->nV = nV;
	g->nE = 0;

	// allocate memory for array of lists
	g->edges = malloc(nV * sizeof(List));
	assert(g->edges != NULL);
	for (i = 0; i < nV; i++) g->edges[i] = NULL;

	return g;
}

// function that fills the adjacency list with url list reps
// input: graph, array of vertices
void fillGraph (Graph g, Vertex* vertices) {
	assert (g != NULL);
	
	// iterate through edges[]
	int vIndex = 0;
	int gIndex;
	for(gIndex = 0; gIndex < g->nV; gIndex++) {
		// if it's empty
		if (g->edges[gIndex] == NULL) {
			// make an empty list rep of that vertex
			List newL = newList(vertices[vIndex], NULL);
			g->edges[gIndex] = newL;
			vIndex ++;
		}
	}
}

// check if vertex is valid in a graph
bool validV(Graph g, Vertex v) {
	return (g != NULL && v != NULL && findIndex(g, v) != -1);
}

void insertEdge(Graph g, Edge e) { 
	assert(g != NULL);
   	if (e.start == e.end) return; // avoid self loops
   	
	int startI = findIndex(g, e.start);
	int endI = findIndex(g, e.end);	
	
	// if either index is -1, what does that mean?
	// that means that vertex is not in the graph
	if (startI == -1 || endI == -1) return;
	
	if (!inLL(g->edges[startI]->first, e.end)) {   // edge e not in graph
		g->edges[startI] = insertLL(g->edges[startI], e.end);
		g->nE++;
	}
}

void removeEdge(Graph g, Edge e) {
	assert(g != NULL && validV(g,e.start) && validV(g,e.end));

	int startI = findIndex(g, e.start);

	if (inLL(g->edges[startI]->first, e.end)) {   // edge e in graph
		g->edges[startI] = deleteLL(g->edges[startI], e.end);
		g->nE--;
	}
}
// function that checks if vertex v is adjacent to vertex w
// aka if w is an outbound link of v
bool adjacent(Graph g, Vertex v, Vertex w) {
	assert(g != NULL && validV(g,v));
	int startI = findIndex(g, v);
	if (inLL(g->edges[startI]->first, w)) {
		return true;
	}
	else return false;
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i;

    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++) {
		if (g->edges[i] == NULL) printf("null - ");
		else {
			printf("%s (%p) - ", g->edges[i]->key, g->edges[i]->key);
			showLL(g->edges[i]->first);
		}
    }
}

void freeGraph(Graph g) {
	assert(g != NULL);
	int i;
	
	for (i = 0; i < g->nV; i++) {
		freeLL(g->edges[i]->first);
		free(g->edges[i]->key);
		free(g->edges[i]);
	}
	free(g->edges);
	free(g);
}

// assumes vertex is in graph
int findIndex(Graph g, Vertex str) {
	int i;
	for (i = 0; i < g->nV; i++) {
		if (isKey(g->edges[i], str)) {
			//printf("found: edges[%d]->key = '%s' ; str = '%s'\n", i, g->edges[i]->key, str);
			return i;
		}
	}
	//printf("did not find index\n");
	return -1;
}

int numVertices(Graph g) {
	return g->nV;	
}

// search through adjacency list, looking for v in each list
// add one for every instance found
double numIn (Graph g, Vertex v) {
	double count = 0;
	int i;
	for (i = 0; i < g->nV; i++) {
		Node curr = g->edges[i]->first; 
		if (inLL(curr, v)) count ++;
	}
	return count;
}

// sum(inlinks(outlinks of v))
double sumNumIn(Graph g, Vertex v) {
	double sum = 0;
	// go to outlinks of v
	int vIndex = findIndex(g,v);
	// traverse outlinks of v
	Node curr;
	for (curr= g->edges[vIndex]->first; curr != NULL; curr = curr->next) {
		// sum the number of inlinks of curr
		sum = sum + numIn(g, curr->v);	
	}
	return sum;
}

// go to v's index and find list length
double numOut(Graph g, Vertex v) {
	int vIndex = findIndex(g, v);
	int length = listLength(g->edges[vIndex]->first);
	if (length == 0) return 0.5;
	else return length;
}

// sum the number of outlinks for each outlink of v
double sumNumOut(Graph g, Vertex v) {
	double sum = 0;
	// go to outlinks of v
	int vIndex = findIndex(g, v);
	// traverse outlinks of v
	Node curr;
	for (curr = g->edges[vIndex]->first; curr != NULL; curr = curr->next) {
		// sum the outlinks of curr
		sum = sum + numOut(g, curr->v);
	}
	return sum;
}
// returns the adjacency list of g
List* adjList(Graph g) {
	return g->edges;
}
