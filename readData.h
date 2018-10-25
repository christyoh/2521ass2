// readData.h ... interface to readData.c

#ifndef READDATA_H
#define READDATA_H

#include "Graph.h"

//#include "invertedIndex.h"

/* Create a set/list of urls to process by reading data from file
 * "collection.txt"
*/
Node getCollection();

/* Create empty graph (graph ADT from graph.h and graph.c)
 * Reads <url>.txt and updates graph by adding nodes and links
*/
Graph getGraph(Node urlList);

// Free list of urls
void freeCollection(Node);
#endif
















