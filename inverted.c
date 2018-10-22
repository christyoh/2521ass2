#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "queue.c"

#define MAX_LINE 1000

typedef struct words {

	int used;
	char *word;
	char **urls;

} Words;

typedef struct wordList {

	int items;
	Words words[];

} wordList;

wordList list;

void trim(char *str);
void add_url(char *url, int i, int x);
void add_word(char *word, char *url, int i);
void sort_list(void);
void print_list(void);
void free_list(void);


int main(void) { //

	int i;
	int x;
	int flag = 0;
	char *tmp;
	char curr_word[MAX_LINE];
	char *curr_url = malloc(1);
	free(curr_url);

	// initialise the list
	list.items = 0;
	list.words[0].used = 0; 

	// open collection
	FILE *file = fopen("collection.txt", "r");
	if (file == NULL) {
		fprintf(stderr, "Couldn't open file 'collection.txt'\n");
		return 1;
	}

	// put all urls in a queue
	Queue q = newQueue();
	char buf[MAX_LINE];
	i = 0;
	while (fscanf(file, "%s", &buf[0]) != EOF) {
		enterQueue(q, buf);
		i ++;
	}
	fclose(file);

	// alphabetically sort urls
	sortQueue(q);

	// while there are still urls to check, continue
	while (!emptyQueue(q)) {

		// get first url and open corresponding .txt file
		tmp = leaveQueue(q);
		curr_url = malloc(sizeof(char)*(strlen(tmp)+5));
		strcpy(curr_url, tmp);
		strcat(curr_url, ".txt");

		file = fopen(curr_url, "r");
		if (file == NULL) {
			printf("Couldn't open file %s\n", curr_url); // ?
			continue;
		}

		// get to required body of text in file
		fscanf(file, "%s", curr_word);
		while (strcmp(curr_word, "Section-2") != 0) {
			fscanf(file, "%s", curr_word);
		}

		// loop to scan each word in the file
		fscanf(file, "%s", curr_word);
		while (strcmp(curr_word, "#end") != 0) {
			trim(curr_word);
			// check if the word is already in list
			// used will be 1 if there is a word in the list index
			for (i = 0, flag = 0; list.words[i].used; i ++) {
				if (strcmp(list.words[i].word, curr_word) == 0) {
					flag = 1;
					break;
				}
			}
			// if word in list, check if url is already in list of urls for that word
			if (flag) {
				for (x = 0, flag = 0; list.words[i].urls[x] != NULL; x ++) {
					if (strcmp(list.words[i].urls[x], tmp) == 0) flag = 1;
				}
				// if url isn't already in list, add it to list
				if (!flag) add_url(tmp, i, x);
			// if word isn't in list, add it to list
			} else {
				add_word(curr_word, tmp, i);
			}
			fscanf(file, "%s", curr_word);
		}
		fclose(file);
		free(tmp);
		free(curr_url);
	}
	free(q);

	sort_list();

	// TESTING
	// for (i = 0; list.words[i].used; i ++) {
	// 	printf("%s ", list.words[i].word);
	// 	for (x = 0; list.words[i].urls[x] != NULL; x ++) {
	// 		printf("%s ", list.words[i].urls[x]);
	// 	}
	// 	printf("\n");
	// }

	print_list();

	free_list();

	return 0;
}


// makes the word lowercase and removes unnecessary spaces and punctuation
void trim(char *str) {

   int first;		// index of first charactedr in word
   int last; 		// index of last character in word

   first = 0;
   while (str[first] == ' ') first++;

   last  = strlen(str)-1;
   while (str[last] == ' ' || str[last] == '.' ||
   		  str[last] == '?' || str[last] == ',')  last--;

   int i = first;
   int j = 0;
   // makes word lowercase and copies across required characters
   while (i <= last) {
   		if (str[i] >= 'A' && str[i] <= 'Z') {
   			str[i] = str[i] - 'A' + 'a';
   		}
   		str[j] = str[i];
   		i ++;
   		j ++;
   }
   str[j] = '\0';
}


// adds url given by 'url' to the list at word index pointed to by 'i'
// and url index given by 'x'
void add_url(char *url, int i, int x) {

	char **tmp_2d;
	int j;
	tmp_2d = malloc(sizeof(char *)*(x+1));

	// put current urls for word at index 'i' into tmp variable
	for (j = 0; list.words[i].urls[j] != NULL; j ++) {
		tmp_2d[j] = strdup(list.words[i].urls[j]);
		free(list.words[i].urls[j]);
	}
	free(list.words[i].urls);
	tmp_2d[j] = NULL;

	// resize urls array and put tmp back into the urls array
	list.words[i].urls = malloc(sizeof(char *)*(x+2));
	for (j = 0; tmp_2d[j] != NULL; j ++) {
		list.words[i].urls[j] = strdup(tmp_2d[j]);
		free(tmp_2d[j]);
	}
	free(tmp_2d);
	list.words[i].urls[j] = strdup(url);
	j ++;
	list.words[i].urls[j] = NULL;
}


// add given 'word' with given 'url' into list at index 'i'
void add_word(char *word, char *url, int i) {

	list.words[i].word = strdup(word);
	list.words[i].urls = malloc(sizeof(char *)*2); // to account for null pointer
	list.words[i].urls[0] = strdup(url);
	list.words[i].urls[1] = NULL;
	list.words[i].used = 1;
	i ++;
	list.words[i].used = 0;
	list.items ++;
}


// alphabetically sorts all words in the list, and all their urls
// basic bubble sort
void sort_list(void) {

	int swap = 1; 	// swap flag
	int i = 0; 		// index of lower word in list
	int x = 1; 		// index of higher word in list
	int j; 			// temporary counter
	Words tmp;		// temporary Words variable

	// while swaps are still being made, continue
	while (swap) {
		swap = 0;
		i = 0;
		x = 1;
		while (x < list.items) {
			// if the former word is higher alphebetically than the latter word, swap them
			if (strcmp(list.words[i].word, list.words[x].word) > 0) {
				swap = 1;

				// put word in 'x' index into tmp and free 'x' index
				tmp.word = strdup(list.words[x].word);
				free(list.words[x].word);

				for (j = 0; list.words[x].urls[j] != NULL; j ++);
				tmp.urls = malloc(sizeof(char *)*(j+1));
				for (j = 0; list.words[x].urls[j] != NULL; j ++) {
					tmp.urls[j] = strdup(list.words[x].urls[j]);
					free(list.words[x].urls[j]);
				}
				free(list.words[x].urls);
				tmp.urls[j] = NULL;

				// put word in 'i' index into 'x' and free 'i' indexx
				list.words[x].word = strdup(list.words[i].word);
				free(list.words[i].word);

				for (j = 0; list.words[i].urls[j] != NULL; j ++);
				list.words[x].urls = malloc(sizeof(char *)*(j+1));
				for (j = 0; list.words[i].urls[j] != NULL; j ++) {
					list.words[x].urls[j] = strdup(list.words[i].urls[j]);
					free(list.words[i].urls[j]);
				}
				free(list.words[i].urls);
				list.words[x].urls[j] = NULL;

				// put tmp into 'i' index and free tmp
				list.words[i].word = strdup(tmp.word);
				free(tmp.word);

				for (j = 0; tmp.urls[j] != NULL; j ++);
				list.words[i].urls = malloc(sizeof(char *)*(j+1));
				for (j = 0; tmp.urls[j] != NULL; j ++) {
					list.words[i].urls[j] = strdup(tmp.urls[j]);
					free(tmp.urls[j]);
				}
				free(tmp.urls);
				list.words[i].urls[j] = NULL;

			}
			i ++;
			x ++;
		}
	}
}


// print list of words and urls into given invertedIndex.txt file
void print_list(void) {
	int i;
	int x;
	FILE *file = fopen("invertedIndex.txt", "w");
	for (i = 0; list.words[i].used; i ++) {
		fprintf(file, "%s ", list.words[i].word);
		for (x = 0; list.words[i].urls[x] != NULL; x ++) {
			fprintf(file, "%s ", list.words[i].urls[x]);
		}
		fprintf(file, "\n");
	}

}


// frees all memory associated with the list
void free_list(void) {
	int i;
	int x;
	for (i = 0; list.words[i].used; i ++) {
		free(list.words[i].word);
		for (x = 0; list.words[i].urls[x] != NULL; x ++) {
			free(list.words[i].urls[x]);
		}
		free(list.words[i].urls);
	}
}