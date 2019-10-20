
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

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
void showPointer(char **pointer,int num_words);
void deletepointers(char **pointer,int num_words);
int count_line_words(char *line);
int process_line(char **words,char *line,int index);
char **process_file(char *file,int *file_words);
void createTree(char *path,char **dic,int dic_size);


int main(int argc, char **argv){
  int dic_size;//indexes for dic
  char **dic;//contains dictionary

  //We load a dic as a pointer.
  dic_size = 0;
  dic_size = countFileWords("/home/yusepp/Documentos/SO2/diccionari/words");
  dic = getWords("/home/yusepp/Documentos/SO2/diccionari/words",dic_size);

  createTree("file",dic,dic_size);
  /* Delete the tree */
  deletepointers(dic,dic_size);

  return 0;
}

void createTree(char *path,char **dic,int dic_size){
  int *file_words = 0;//how many words in file
  char **file;//contains words in file
  char *a;//auxiliar for node key
  int  ct;//counter of nodes

  //We load a file as a pointer.
  file_words = malloc(sizeof(int));
  file = process_file(path,file_words);

  rb_tree *tree;//tree
  node_data *n_data;//node

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(tree);
  printf("Creating: %s Tree\n",path);

  //We start filling the tree
  for (ct = 0; ct < *file_words; ct++) {//every word in file
    for(int j = 0; j < dic_size; j++){//every word in dic

      if(strcasecmp(file[ct], dic[j]) == 0){//compare each other
        //if a the word its in dic we add it
        a = dic[j];
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
      
    }

  }
  
  //We show the tree
  ct = 0;
  int i = 0;

  for(i = 0; i <= dic_size; i++)
  {
  	a = dic[i];
    n_data = find_node(tree, a);

    if (n_data) { 
      printf("La paraula %s apareix %d cops a l'arbre.\n", a,n_data->num_times);
      ct += n_data->num_times;
    }
  }

  printf("Nombre total que vegades que s'ha accedit a l'arbre: %d\n", ct);

  //delete tree
  deletepointers(file,*file_words);
  delete_tree(tree);
  free(tree);
  free(a);
  free(n_data);
  free(file_words);
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
	int i;

	fp = fopen(file,"r");//read file
	//checking for errors at reading
	if(fp == NULL){
		printf("Error opening file\n");
		exit(1);
	}
	tmp = malloc(sizeof(char) * MAXCHAR);
	//Now we can malloc our main pointer
	getDicWords = malloc(num_words*sizeof(tmp));
	//and in the subpointers
	i = 0;
	//we now must start reading
	while (fgets(tmp, MAXCHAR, fp)){
		//printf("%s\n",tmp);
    if(tmp != NULL){
      tmp[strlen(tmp)-1] = '\0';
		  getDicWords[i] = strdup(tmp);
		  i++;
    }
   
	}
  free(tmp);
	fclose(fp);
	//return pointer
	return getDicWords;



}

void showPointer(char **pointer,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		printf("%s\n",pointer[i]);
	}
}

void deletepointers(char **pointer,int num_words){
	int i;
	for(i = 0; i< num_words; i++){
		free(pointer[i]);
	}
	free(pointer);
}


int count_line_words(char *line)
{
    int i, j, is_word, len_line,words_found;
    i = 0;
    char *paraula;
    paraula = malloc(MAXCHAR * sizeof(char));
    len_line = strlen(line);

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    /* This is the main loop that extracts all the words */

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        /* Extract the candidate word including digits if they are present */

        do {
            if(isalpha(line[i]) || isdigit(line[i]) || ispunct(line[i])){
                paraula[j] = line[i];
            }
            else{ 
                is_word = 0;
            }
            j++; i++;


            /* Check if we arrive to an end of word: space or punctuation character */

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i]) || (line[i] == '\'') || (line[i] == '-')));

        /* If word insert in list */

        if (is_word) {
            words_found++;
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    } /* while (i < len_line) */
    free(paraula);
    return words_found;
}


int process_line(char **words,char *line,int index){

    int i, j, is_word, len_line,words_found;
    char *paraula;
    i = 0;
    paraula = malloc(MAXCHAR*sizeof(char));
    len_line = strlen(line);

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    /* This is the main loop that extracts all the words */

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        /* Extract the candidate word including digits if they are present */

        do {
            if(isalpha(line[i]) || isdigit(line[i]) || ispunct(line[i])){
                paraula[j] = line[i];
            }
            else{ 
                is_word = 0;
            }
            j++; i++;

            /* Check if we arrive to an end of word: space or punctuation character */

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i]) || (line[i] == '\'') || (line[i] == '-')));

        /* If word insert in list */

        if (is_word) {
            /* Put a '\0' (end-of-word) at the end of the string*/
            paraula[j] = '\0';
            words[index] = strdup(paraula);
            //printf("%s\n",words[index]);
            index++;
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    } /* while (i < len_line) */
    free(paraula);
    return index;
}


char **process_file(char *file,int *file_words){
    FILE *fp;
    char **words;
    char *tmp;
    int index = 0,i;
    int total_words = 0;
    
    fp = fopen(file, "r");
    if (!fp) {
        printf("Could not open file\n");
        exit(1);
    }
    
    tmp = malloc(MAXCHAR*sizeof(char));

    while (fgets(tmp, MAXCHAR, fp)){
      if(tmp != NULL){
        total_words += count_line_words(tmp);
      }
    }
        

    words = malloc(total_words*sizeof(tmp));
    //and in the subpointers
  
    i = 0;
    rewind(fp);

    while (fgets(tmp, MAXCHAR, fp)){
      if(tmp != NULL){
        index = process_line(words,tmp,index);
      }
    }
        
    
    *file_words = total_words;
    free(tmp);
    fclose(fp);
    return words;
}



