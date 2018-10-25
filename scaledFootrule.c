#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <math.h>
#include "set.h"

#define LINESIZE 1000

Set readRankList (int argc, char* argv[]);
double calculateScaledFootRule (Set setC, int fileIndex, char* elem, int pos);
void heapPermutation(int a[], int size, int sizeC, Set setC, int optPos[], int nFiles);
void swap(int* a, int* b);
void writeToOutput (Set S, int optPos[]);

double opt = -1;

int main (int argc, char *argv[]) {
    
    if (argc < 3) {
    	fprintf(stderr, "Error in input format: ./readRankList <rankList> <rankList> ...\n");
    	abort();
    } 

	int nFiles = argc-1;

	//read in rank list files 
	Set setC = readRankList (argc, argv);
	int sizeC = sizeSet(setC);
	
	// setC is the set of different urls
	// it stores each url's ranks for each file
	
	// permute different arrangements for urls in setC
	int* cPos = calloc(sizeC, sizeof(int));
	int i;
	for (i = 0; i < sizeC; i++) cPos[i] = i+1;
	
	// intialise optimum positions to be starting array
	int* optPos = calloc(sizeC, sizeof(int));
	for (i = 0; i < sizeC; i++) optPos[i] = cPos[i];

	heapPermutation(cPos, sizeC, sizeC, setC, optPos, nFiles);

	writeToOutput(setC, optPos);
	
	// free everything
	free(cPos);
	free(optPos);
	freeSet(setC);
    return EXIT_SUCCESS;
}


// read rank lists into a set
Set readRankList (int argc, char* argv[]){
	Set setC = NULL;

	int nFiles = argc-1;
	// for each rank file
	int rankList;
	for (rankList = 1; rankList < argc; rankList++) {
		// open the rank file
		FILE *fp = fopen(argv[rankList], "r");
		int rank = 1;
		char str[LINESIZE] = {'\0'};
		while (fscanf(fp, "%s \n", str) != EOF) {
			// insert into the set and update the rank of the url
			if (setC == NULL) {
				Node newNode = makeNode(str, nFiles);
				newNode->ranks[rankList-1] = rank;
				setC = newNode;
			}
			else {
				insertSet(setC, str, rankList-1, rank, nFiles);
		    }
		    rank++;
		}
		fclose(fp);	
	}
	return setC;
}


// function that:
// 1. generates a permutation of a[]
// 2. calculates SFR for that permutation
// 3. updates optSFR and optArray
// inputs: a[], permutation size, size of C sizeC, current optimum opt,
//		   set of urls setC, current optimum positions optPos[], nFiles,
//		   
// adapted from geeksforgeeks
// https://www.geeksforgeeks.org/heaps-algorithm-for-generating-permutations/
void heapPermutation(int a[], int size, int sizeC, Set setC, int optPos[], int nFiles) 
{ 
    // if size becomes 1 then copies the obtained permutation into storage
    int i;
    if (size == 1) {
		// total is the total weight for this position array
		double total = 0;
		int pos;
		for (pos = 0; pos < sizeC; pos++) {
			//find url at position pos
			char* url = findValue(setC, a[pos]);
			//for every fileIndex in nFiles-1
			int fileIndex;
			for (fileIndex = 0; fileIndex < nFiles; fileIndex++) {
				// if the url is not in the index, just skip and don't calculate
				if (posRF(setC, url, fileIndex) == 0) continue;
				// calculate Scaled FootRule
				double SFR = calculateScaledFootRule (setC, fileIndex, url, pos+1);
				total += SFR;
			}
		}
		// check if found new optimum OR no optimum yet
		if (total < opt || opt < 0) {
			// update optimum
			opt = total;
			// update optPos
			for (i=0; i<sizeC; i++) {
				optPos[i] = a[i];
			} 
		}
		
        return; 
    } 
    for (i=0; i<size; i++) { 
  
        heapPermutation(a,size-1,sizeC, setC, optPos, nFiles); 
        // if size is odd, swap first and last 
        // element 
        if (size%2==1) 
            swap(&a[0], &a[size-1]); 
  
        // If size is even, swap ith and last element 
        else
            swap(&a[i], &a[size-1]); 
    } 
} 

// swaps two items
void swap(int* a, int* b) {
	int c = *b;
	*b = *a;
	*a = c;
}

// calculate the scaled foot rule for an element at position p
double calculateScaledFootRule (Set setC, int fileIndex, char* elem, int pos) {
	double setSize = (double) sizeSet(setC);
	double rankFileSize = (double) sizeRF(setC, fileIndex);
	double rankFilePos = (double) posRF(setC, elem, fileIndex);
	
	double scaledFootRule = fabs(rankFilePos/rankFileSize - ((double) pos)/setSize);

	return scaledFootRule;
}

// function that writes to output file scaledRankList.txt
void writeToOutput (Set S, int optPos[]) {
	assert(S != NULL);

	FILE* outputStream = fopen("scaledRankList.txt", "w");

	fprintf(outputStream, "%.6lf\n", opt);
	
	fclose (outputStream);
	
	outputStream = fopen("scaledRankList.txt", "a");
	int i;
	for (i = 0; i < sizeSet(S); i++) 
		fprintf(outputStream, "%s\n", findValue(S, optPos[i]));
		
	fclose(outputStream);
}
