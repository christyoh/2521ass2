// Graph ADT interface
// Taken from COMP2521 wk05 lecture (ashesh)

#include <stdbool.h>

typedef struct GraphRep *Graph;

// vertices are strings
typedef char* Vertex;

// edges are pairs of vertices (end-points)
typedef struct Edge {
   Vertex v;
   Vertex w;
} Edge;

Graph newGraph(char*);
void  insertEdge(Graph, Edge);
void  removeEdge(Graph, Edge);
bool  adjacent(Graph, Vertex, Vertex);
void  showGraph(Graph);
void  freeGraph(Graph);
