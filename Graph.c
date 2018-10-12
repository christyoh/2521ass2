/*
Graph structure-based search engine
Create a graph structure that represents a hyperlink structure of
	given collection of "web pages"
For each page (node in your graph):
	calculate Weighted PageRank
	and other graph properties
Create "inverted index" that provides a list of pages
	for every word in a given collection of pages.
Search engine will use this inverted index to
	find pages where query term(s) appear
	and rank these pages (Weighted PageRank values). 
*/

// Altered version of Graph.c from 2521 wk05 lecture
// Altered by Victor Tsang (z5209633)
// Date: 11 Oct 2018

#include "Graph.h"
#include "list.h"
#include <assert.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct GraphRep {
   List *edges;  // array of lists
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
   for (i = 0; i < nV; i++)
      g->edges[i] = NULL;

   return g;
}

// check if vertex is valid in a graph
bool validV(Graph g, Vertex v) {
   return (g != NULL && v >= 0 && v < g->nV);
}

void insertEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (!inLL(g->edges[e.v], e.w)) {   // edge e not in graph
      g->edges[e.v] = insertLL(g->edges[e.v], e.w);
      g->edges[e.w] = insertLL(g->edges[e.w], e.v);
      g->nE++;
   }
}

void removeEdge(Graph g, Edge e) {
   assert(g != NULL && validV(g,e.v) && validV(g,e.w));

   if (inLL(g->edges[e.v], e.w)) {   // edge e in graph
      g->edges[e.v] = deleteLL(g->edges[e.v], e.w);
      g->edges[e.w] = deleteLL(g->edges[e.w], e.v);
      g->nE--;
   }
}

bool adjacent(Graph g, Vertex v, Vertex w) {
   assert(g != NULL && validV(g,v));

   return inLL(g->edges[v], w);
}

void showGraph(Graph g) {
    assert(g != NULL);
    int i;

    printf("Number of vertices: %d\n", g->nV);
    printf("Number of edges: %d\n", g->nE);
    for (i = 0; i < g->nV; i++) {
       printf("%d - ", i);
       showLL(g->edges[i]);
    }
}

void freeGraph(Graph g) {
   assert(g != NULL);
   int i;

   for (i = 0; i < g->nV; i++)
      freeLL(g->edges[i]);

   free(g);
}
