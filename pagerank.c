#include <stdlib.h>
#include <stdio.h>
#include <assert.h>
#include <math.h>
#include "pagerank.h"

int main(int argc, char* argv[]) {
	// collect command-line params
	double d; // damping factor
	double diffPR; // difference in PageRank sum
	int maxIterations; // maximum iterations
	
	if (argc != 4) {
		fprintf(stderr, "Error: Incorrect number of arguments supplied. \nFormat should be: ./pagerank <d> <diffPR> <maxIterations>\n");
		abort();
	}

	d = atof(argv[1]);
	diffPR = atof(argv[2]);
	maxIterations = atoi(argv[3]);
	
	Node url_list = getCollection();
	Graph g = getGraph(url_list);
	List* updated_url_list = calculatePageRank (g, d, diffPR, maxIterations);
	List* ordered_url_list = order( g, updated_url_list);

	writeOut(g, ordered_url_list);
	
	//free everything
	freeCollection(url_list);
	freeGraph(g);
	return 0;
}

/* This function traverses a graph g
 * calculates pageranks for each vertex
 *
 * and returns the adjacency list, with updated pageranks
 * each index corresponds to each node in the graph
*/
List* calculatePageRank (Graph g, double d, double diffPR, int maxIterations) {
	int N = numVertices(g);
	double Ndouble = (double) N;
	
	// copy g->edges into an array of lists
	List* edges = adjList(g);
	
	// set all pageranks to 1/N
	int i;
	for (i = 0; i < N; i++)	edges[i]->pagerank = 1.00/Ndouble;
	
	// variables for the loop
	int iteration = 0;
	double diff = diffPR; // to enter the loop
	double* oldPR = calloc(N, sizeof(double));
	assert(oldPR != NULL);
	
	while (iteration < maxIterations && diff >= diffPR) {
		// update oldPR with current pagerank 
		for (i = 0; i < N; i++) oldPR[i] = edges[i]->pagerank;
		// pagerank calculation
		// for every url in pagerank
		for (i = 0; i < N; i++) {
			//initalise PRsum = 0
			double PRsum = 0;
			// find all outgoing links to current url
			int j;
			for (j = 0; j < N; j++) {
				// if j has outgoing links to i
				if (adjacent(g, edges[j]->key, edges[i]->key)) {
					// calculate PRsum
					double inWeight = wIn(g, edges[j]->key, edges[i]->key);
					double outWeight = wOut(g, edges[j]->key, edges[i]->key);
					PRsum = PRsum + (oldPR[j]*inWeight*outWeight);
				}		
			}
			edges[i]->pagerank = (1.00-d)/Ndouble + d*PRsum;
		}

		// for every url, sum abs(pagerank[i] - oldPR[i])
		double diffSum = 0;
		for (i = 0; i < N; i++) {
			diffSum = diffSum + fabs(edges[i]->pagerank - oldPR[i]);
		}
		diff = diffSum;
		iteration++;
	}
	free(oldPR);
	return edges;
}

// weight of link (v,u) based on inlinks
// wOut(v,u) = inlinks(u)/sum(inlinks(outlinks of v))
double wIn(Graph g, Vertex v, Vertex u) {
	return numIn(g,u)/sumNumIn(g,v);
}

// weight of link (v,u) based on outlinks
// wOut(v,u) = outlinks(u)/sum(outlinks(every outlink of v))
double wOut(Graph g, Vertex v, Vertex u) {
	return numOut(g, u)/sumNumOut(g, v);
}

// function that sorts the adjacency list of graph g
// takes in graph g and the pagerank
List* order(Graph g, List* urlList) {
	int n = numVertices(g);
	qsort(urlList, n, sizeof(List), PRCompare);
	return urlList;	
}

// function that compares two list structs using their pagerank values
// returns -1 if list1 > list2
// returns 0 if list1 == list2
// returns 1 if list1 < list2
int PRCompare(const void* list1, const void* list2) {
	List listA = *(List*) list1;
	List listB = *(List*) list2;
	
	if (listA->pagerank > listB->pagerank) return -1;
	else if (listA->pagerank < listB->pagerank) return 1;
	else return 0;
}

// output the results
void writeOut(Graph g, List* ordered_url_list) {
	FILE* outputStream = fopen("pagerankList.txt", "w");
	int i;
	for (i = 0; i < numVertices(g); i++) {
		Vertex url = ordered_url_list[i]->key;
		double outCount = numOut(g, url);
		double prValue = ordered_url_list[i]->pagerank;
		fprintf(outputStream, "%s, %d, %.7lf\n", url, (int) outCount, prValue);
	}
	fclose(outputStream);
}








