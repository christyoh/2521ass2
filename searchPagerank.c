/*
COMP2521 Assignment 2 2018s2

A simple search engine that given search terms (words) as commandline
arguments, finds pages with one or more search terms and outputs
(to stdout) top 30 pages in descending order of number of search terms
found and then within each group, descending order of Weighted PageRank.
If number of matches are less than 30, output all of them.

Get query words from arguments
Output ordered urls on stdout
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "searchPagerank.h"
#include "newlist.h"

typedef struct Node {
   int           nterms;
   UrlList		 urls;
   struct Node  *next;
} Node;

typedef struct Url {
   char*	   v;
   struct Url *next; 
} Url;

int searchPagerank(int argc, char *argv)
{
    
    argCheck(argc);
    
    // Malloc array for query words
    int numWords = argc - 1;
    char **queryWords = malloc(sizeof(char *) * numWords);
    int i = 0;
    while (i < numWords) {
        queryWords[i] = malloc(strlen(argv[i+1]) * sizeof(char));
        i++;
    }
    
    // Create array of query words to pass into findMatchedUrls
    i = 0;
    while (i < numWords) {
        strcpy(queryWords[i], argv[i+1]);
        i++;
    }
    
    // Get combined list of urls for each search term
    UrlList matches = findMatchedUrls(numWords, queryWords);
    if (matches == NULL) {
        return 0;
    }
    
    // Create List L ordered by num of search terms with pointers to
    // lists of urls for each num of search terms
    List L = createList(matches);
    
    // Order each List of urls by their pagerank and print
    orderURLs(L);
    
    // Free queryWords
    freeQuery(queryWords, numWords);
    
    freeLL(matches);
    freeLL1(L);
    
    return 0;
    
}

UrlList findMatchedUrls(int numWords, char *queryWords[])
{
    
    char *str = malloc(sizeof(char));
    char c;
    
    FILE *fp;
    fp = fopen("invertedIndex.txt", "r");
    
    // return List of urls that match
    UrlList L = NULL;
    int i = 0;
    while (i < numWords) {
        rewind(fp);
        while (fscanf(fp, "%s", str)) {
            if(feof(fp)) { 
                break;
            }
            if (strcmp(str, queryWords[i]) == 0) {
                // copy urls into final List
                fscanf(fp, "%c", &c);
                while (c != '\n') {
                    fscanf(fp, "%s", str);
                    L = insertLL(L, str);
                    fscanf(fp, "%c", &c);
                }
            }
        }
        i++;
    }
    fclose(fp);
    
    free(str);
    
    return L;
    
}

List createList(UrlList matches)
{
    
    // Add urls to LL sorted by number of search terms
    UrlList curr = matches;
    UrlList temp = curr;
    List L = NULL;
    int count, i;
    while (curr->next != NULL) {
        temp = curr->next;
        count = 1;
        while (temp != NULL) {
            if (strcmp(curr->v, temp->v) == 0) {
                count++;
            }
            temp = temp->next;
        }
        // insert each url once into LL
        L = insertLL1(L, count, curr->v);
        i = 0;
        while (i < count) {
            matches = deleteLL(matches, curr->v);
            i++;
        }
        curr = matches;
    }
    // curr->next is NULL
    L = insertLL1(L, 1, curr->v);
    freeLL(matches);
    free(temp);
    free(curr);
    
    return L;
    
}

// order each List of urls by their pagerank
void orderURLs(List L)
{

    List tmp = L;
    int count = 0;
    while (tmp != NULL) {
        count = findPagerank(tmp->urls, count);
        if (count > 29) {
            break;
        }
        tmp = tmp->next;
    }
    
}

int findPagerank(UrlList L, int count)
{
    
    char *str = malloc(sizeof(char));
    FILE *fp;
    fp = fopen("pagerankList.txt", "r");
    
    UrlList curr = L;
    
    while (fscanf(fp, "%s", str)) {
        str = strtok(str, ",");
        if(feof(fp)) {
            break;
        } else {
            curr = L;
            while (curr != NULL) {
                if (strcmp(str, curr->v) == 0 && count < 30) {
                    printf("%s\n", str);
                    count++;
                    break;
                }
                curr = curr->next;
            }
        }
    }
    
    fclose(fp);
    
    free(curr);
    free(str);
    freeLL(L);
    
    return count;
    
}

// Check arguments
void argCheck(int argc)
{
    if (argc <= 1) {    // no search terms
        printf("Usage: ./searchPagerank <term>\n");
    }
}

// Free queryWords
void freeQuery(char **queryWords, int numWords)
{
    int i = 0;
    while (i < numWords) {
        free(queryWords[i]);
        i++;
    }
    free(queryWords);
}
