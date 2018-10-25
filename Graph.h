// Graph ADT interface
// Taken from COMP2521 wk05 lecture (ashesh)

#ifndef GRAPH_H
#define GRAPH_H

#include <stdbool.h>
#include "list.h"
typedef struct GraphRep *Graph;

// vertices are strings
typedef char* Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex start;
   Vertex end;
} Edge;

// graph operations
Graph newGraph(int);
void  fillGraph (Graph, Vertex*);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);
int   findIndex(Graph, Vertex);
int   numVertices(Graph);
double numIn(Graph, Vertex);
double sumNumIn(Graph, Vertex);
double numOut(Graph, Vertex);
double sumNumOut(Graph, Vertex);
List* adjList(Graph);
#endif
