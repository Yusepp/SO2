/**
 *
 * Main file 
 * 
 * This file is an example that uses the red-black-tree.c functions.
 *
 * Lluis Garrido, July 2019.
 *
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include "red-black-tree.h"

#define MAXVALUE 10
#define MAXCHAR 100

/**
 *
 *  Main function. This function is an example that shows
 *  how the binary tree works.
 *
 */

int countFileWords(char *file);
char **getWords(char * file,int num_words);
void showdict(char **dict,int num_words);
void deletedict(char **dict,int num_words);


int main(int argc, char **argv){
  int  ct,maxnum;
  char **words;
  char *a;
  rb_tree *tree;
  node_data *n_data;

  printf("Test with red-black-tree\n");

  /* Random seed */
  srand(time(NULL));

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));

  //We load a file as pointer.
  maxnum = 0;
  maxnum = countFileWords("/home/yusepp/Documentos/SO2/P2/diccionari/words");
  words = getWords("/home/yusepp/Documentos/SO2/P2/diccionari/words",maxnum);//
  /* Initialize the tree */
  init_tree(tree);

  for (ct = 0; ct < maxnum; ct++) {
    /* Key from dictionary */
    a = words[ct];

    /* Search if the key is in the tree */
    n_data = find_node(tree, a); 

    if (n_data != NULL) {

      /* If the key is in the tree increment 'num' */
      n_data->num_times++;
    } else {

      /* If the key is not in the tree, allocate memory for the data
       * and insert in the tree */

      n_data = malloc(sizeof(node_data));
      
      /* This is the key by which the node is indexed in the tree */
      n_data->key = a;
      
      /* This is additional information that is stored in the tree */
      n_data->num_times = 1;

      /* We insert the node in the tree */
      insert_node(tree, n_data);
    }
  }
  
  /* We now dump the information of the tree to screen */

  ct = 0;
  int i = 0;

  for(i = 0; i <= maxnum; i++)
  {
  	a = words[i];
    n_data = find_node(tree, a);

    if (n_data) { 
      printf("La paraula %s apareix %d cops a l'arbre.\n", a,n_data->num_times);
      ct += n_data->num_times;
    }
  }

  printf("Nombre total que vegades que s'ha accedit a l'arbre: %d\n", ct);
  
  /* Delete the tree */
  delete_tree(tree);
  free(tree);
  free(a);
  deletedict(words,maxnum);

  return 0;
}

int countFileWords(char *file){
	FILE *fp;
	char *word;
	int counter;
	fp = fopen(file, "r");
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}

	//Counting how many words we gonna read.
	counter = 0;
	word = malloc(sizeof(char) * MAXCHAR);
	while (fgets(word, MAXCHAR, fp)){
		counter++;
	}
	free(word);
	fclose(fp);
	return counter-1;

}

char **getWords(char * file,int num_words){
	//Initialize variables
	char ** getDicWords;
	char *tmp;
	FILE *fp;
	char *word;
	int i;

	fp = fopen(file,"r");//read file
	//checking for errors at reading
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	tmp = malloc(sizeof(char) * MAXCHAR);
	//Now we can malloc our main pointer
	getDicWords = malloc(num_words*sizeof(*getDicWords));
	//and in the subpointers
	for(i = 0; i < num_words; i++){
		getDicWords[i] = malloc((MAXCHAR + 1) * sizeof(**getDicWords));
	}
	i = 0;
	//we now must start reading
	while (fgets(tmp, MAXCHAR, fp)){
		//printf("%s\n",tmp);
		getDicWords[i] = strdup(tmp);
		i++;
	}
	fclose(fp);
	//return pointer
	return getDicWords;



}

void showdict(char **dict,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		printf("%s\n",dict[i]);
	}
}

void deletedict(char **dict,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		free(dict[i]);
	}
	free(dict);
}



