// header file for pagerank.c

#include <stdlib.h>
#include <stdbool.h>
#include "readData.h"

List* calculatePageRank (Graph g, double d, double diffPR, int maxIterations);
double wIn(Graph g, Vertex v, Vertex u);
double wOut(Graph g, Vertex v, Vertex u);
List* order(Graph, List*);
int PRCompare(const void*, const void*);
