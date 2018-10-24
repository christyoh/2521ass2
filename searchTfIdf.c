// COMP2521 Assignment 2, 2018 s2
// Written by Erin Leavy, z5163470
// 20 October 2018

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#define MAX_LINE 1000

typedef struct _url_info {

	char *url;
	int no_terms;
	float tf; 		// a temporary variable for use in functions
	float *tfidf; 	// a list of tfidfs for each relevant word

} Info;

typedef struct _all_urls {

	int items;
	Info url_info[];

} Urls;

typedef struct _collection {

	int *no_urls;
	char **collection_urls; // all urls in collection
	float *tfidfs; 			// a list of tfdifs corresponding to each url

} Collection;

// taken from the official GNU C String Library
// Copyright (C) 1992-2018 Free Software Foundation, Inc.
// duplicates the given string
static char *strdup (const char *s)
{
  size_t len = strlen (s) + 1;
  void *new = malloc (len);
  if (new == NULL)
    return NULL;
  return (char *) memcpy (new, s, len);
}

// taken from the official GNU C String Library
// Copyright (C) 1992-2018 Free Software Foundation, Inc.
// separates given string at delim and returns separated strings
static char *strsep (char **stringp, const char *delim)
{
  char *begin, *end;
  begin = *stringp;
  if (begin == NULL)
    return NULL;
  /* Find the end of the token.  */
  end = begin + strcspn (begin, delim);
  if (*end)
    {
      /* Terminate the token and set *STRINGP past NUL character.  */
      *end++ = '\0';
      *stringp = end;
    }
  else
    /* No more delimiters; this is the last token.  */
    *stringp = NULL;
  return begin;
}

Urls urls;
Collection collection;

int url_check(char *str);
float calculate_tf(char *url1, char *word);
void trim(char *str);
void total_idfs(void);
void sort_idfs(int file_num);
void print_idfs(int file_num);
void free_urls(void);
void free_array(char **array, int file_num);
void free_collection(int file_num);

int main (int argc, char *argv[]) {

	if (argc < 1) {
		printf("Please enter a valid search term\n");
		return 1;
	}

	// count how many files there are total
	FILE *collection_f = fopen("collection.txt", "r");
	if (collection_f == NULL) {
		printf("Couldn't open file 'collection.txt'\n");
		return 1;
	}

	char buf[MAX_LINE];
	float file_num = 0;
	while (fscanf(collection_f, "%s", buf) != EOF) {
		file_num ++;
	}
	fclose(collection_f);

	// store all argv words in a all_words list
	int i = 1;
	char **all_words = malloc(sizeof(char *)*(argc+1));
	all_words[0] = NULL;
	while (i < argc) {
		all_words[i-1] = strdup(argv[i]);
		all_words[i] = NULL;
		i ++;
	}

	// loop through all search terms
	FILE *index;
	float url_count = 0; // counts urls associated with each term
	float idf; 			 // tmp variable that count idf for each word
	float tf = 0; 		 // tmp variable that count tf for each url
	char *curr_url; 	 // current url that's open
	char *all_urls;		 // a copy for the buf, that holds the full string of urls
	int check; 			 // returns from check function
	int j; 				 // index of tfidf array in each url_info
	int x; 				 // index of url_info

	for (i = 0; all_words[i] != NULL; i ++) {
		index = fopen("invertedIndex.txt", "r");
		if (index == NULL) {
			printf("File 'invertedIndex.txt' could not be opened\n");
			return 1;
		}
		fscanf(index, "%s", buf);
		// get all urls with current word in them and calculate idf for this word
		while (fscanf(index, "%s", buf) != EOF && strcmp(all_words[i], buf) != 0);
		if (strcmp(all_words[i], buf) != 0) {
			fclose(index);
			continue;
		}
		fgets(buf, MAX_LINE, index);
		all_urls = strdup(buf);
		// count urls relevant for this word
		for (url_count = -2; (curr_url = strsep(&all_urls, " ")); url_count ++);
		// -2 to account for the initial space in string and the /n at the end
		idf = log10(file_num / url_count);
		free(all_urls);

		// open each url, add to struct, calculate tf and tfidf
		all_urls = strdup(buf);
		curr_url = strsep(&all_urls, " ");
		while ((curr_url = strsep(&all_urls, " ")) && strcmp(curr_url, "\n") != 0) {
			check = url_check(curr_url);
			// check gets index of url array
			// if it's not there, add it to array
			if (check == -1) {
				x = urls.items;
				urls.items ++;
				urls.url_info[x].url = strdup(curr_url);
				urls.url_info[x].tfidf = malloc(sizeof(float)*(argc+1));
				urls.url_info[x].tfidf[0] = 0;
				urls.url_info[x+1].url = NULL;
				urls.url_info[x].no_terms = 1;
			} else {
				x = check;
				urls.url_info[x].no_terms ++;
			}
			// calculate tfidf and store where required
			tf = calculate_tf(curr_url, all_words[i]);
			for (j = 0; urls.url_info[x].tfidf[j] != 0; j ++);
			urls.url_info[x].tfidf[j] = (tf*idf);
			urls.url_info[x].tfidf[j+1] = 0;
		}
		free(all_urls);
		fclose(index);
	}

	// add up all tfidfs for each url and store it in the 'tf' float
	total_idfs();

	// make a collection of all urls and corresponding tfidfs
	collection.collection_urls = malloc(sizeof(char *) * file_num);
	collection.tfidfs = malloc(sizeof(float *) * file_num);
	collection.no_urls = malloc(sizeof(int *) * file_num);
	// copy across al data from url_info array
	for (i = 0; i < urls.items; i ++) {
		collection.collection_urls[i] = strdup(urls.url_info[i].url);
		collection.tfidfs[i] = urls.url_info[i].tf;
		collection.no_urls[i] = urls.url_info[i].no_terms;
	}
	int flag = 0;
	collection_f = fopen("collection.txt", "r");
	// copy across all urls that weren't used in search
	while (fscanf(collection_f, "%s", buf) != EOF) {
		flag = 0;
		for (x = 0; x < urls.items; x ++) {
			if (strcmp(buf, urls.url_info[x].url) == 0) {
				flag = 1;
			}
		}
		if (!flag) {
			collection.collection_urls[i] = malloc(sizeof(char)*(strlen(buf)+1));
			strcpy(collection.collection_urls[i], buf);
			collection.tfidfs[i] = 0;
			collection.no_urls[i] = 0;
			i ++;
		}
	}
	// sort all urls by search term number, tfidf, and alphabet
	sort_idfs(file_num);

	// print all urls and tfidfs required
	print_idfs(file_num);

	// free all data
	free_array(all_words, argc-1);
	free_urls();
	free_collection(file_num);

	return 0;
}

// sort all urls first by number of search terms matched, tfidf, and by alphabet
void sort_idfs(int file_num) {

	if (file_num == 1) {
		return;
	}
	float tmp_f;
	char *tmp_c;
	int i; // index of lower url
	int x; // index of higher url
	int swap = 1;
	while (swap) {
		swap = 0;
		for (i = 0, x = 1; x < file_num; i ++, x ++) {
			// if one has a higher tfidf OR if it's the same tfidf but one is higher alphabetically
			// OR if one has more search terms used, swap them
			if ((collection.no_urls[i] < collection.no_urls[x]) || (collection.tfidfs[i] < collection.tfidfs[x]) ||
				(collection.tfidfs[i] == collection.tfidfs[x] &&
				strcmp(collection.collection_urls[i], collection.collection_urls[x]) > 0)) {
				swap = 1;
				// put x into tmp
				tmp_c = strdup(collection.collection_urls[x]);
				free(collection.collection_urls[x]);
				tmp_f = collection.tfidfs[x];

				// put i into x
				collection.collection_urls[x] = strdup(collection.collection_urls[i]);
				free(collection.collection_urls[i]);
				collection.tfidfs[x] = collection.tfidfs[i];

				// put tmp into i
				collection.collection_urls[i] = strdup(tmp_c);
				free(tmp_c);
				collection.tfidfs[i] = tmp_f;
			}
		}
	}
}

// print all urls and tfidfs as required
// max of 30 urls printed
void print_idfs(int file_num) {

	int i;
	for (i = 0; i < file_num && i < 30; i ++) {
		printf("%s %.6f\n", collection.collection_urls[i], collection.tfidfs[i]);

	}
}

// counts all given tfidfs and stores total in the tf of each url_info struct
void total_idfs(void) {
	int i;
	int x;
	for (i = 0; i < urls.items; i ++) {
		urls.url_info[i].tf = 0;
		for (x = 0; urls.url_info[i].tfidf[x] != 0; x ++) {
			urls.url_info[i].tf += urls.url_info[i].tfidf[x];
		}
	}
}

// if url is in list, returns required index
// else, return -1
int url_check(char *str) {

	int i;
	int num = -1;
	for (i = 0; urls.url_info[i].url != NULL; i ++) {
		if (strcmp(str, urls.url_info[i].url) == 0) {
			num = i;
		}
	}
	return num;
}

// calculates the tf for the word given by 'word' in file given by 'url1'
float calculate_tf(char *url1, char *word) {

	char *url = strdup(url1);
	url = realloc(url, sizeof(char)*(strlen(url1)+4));
	char str[MAX_LINE];
	strcat(url, ".txt");
	FILE *file = fopen(url, "r");
	if (file == NULL) {
		printf("Unable to open file '%s'\n", url);
		return -1;
	}
	// get to required section
	fscanf(file, "%s", str);
	while (strcmp(str, "Section-2") != 0) {
		fscanf(file, "%s", str);
	}

	// count all instances of word in section
	float word_count = 0;
	float str_count = 0;
	fscanf(file, "%s", str);
	while (strcmp(str, "#end") != 0) {
		trim(str);
		if (strcmp(word, str) == 0) {
			word_count ++;
		}
		str_count ++;
		fscanf(file, "%s", str);
	}
	fclose(file);
	free(url);

	// calculate tf
	return word_count / str_count;
}


// free all data associated with url_info array
void free_urls(void) {

	int i;
	for (i = 0; i < urls.items; i ++) {
		free(urls.url_info[i].url);
		free(urls.url_info[i].tfidf);
	}
}

// free all data associated with collection struct
void free_collection(int file_num) {
	free(collection.tfidfs);
	free(collection.no_urls);
	free_array(collection.collection_urls, file_num);
}

// a function to free a given 2d char array
void free_array(char **array, int file_num) {

	int i;
	for (i = 0; i < file_num; i ++) {
		free(array[i]);
	}
	free(array);
}

// makes the word lowercase and removes unnecessary spaces and punctuation
// taken from inverted.c, written by Erin Leavy for Assignment 2 COMP2521, 18 s2
void trim(char *str) {

   int first;		// index of first character in word
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
