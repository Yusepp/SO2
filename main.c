
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"
#include "read_tools.h"

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/words"

/**
 *
 *  Main function. This function is an example that shows
 *  how the binary tree works.
 *
 */


char * createPath(char *start,char *subpath);
void createTree(char *path,char **dic,int dic_size,int list_size,int count,char* mode);


int main(int argc, char **argv){
  int dic_size,list_size;//indexes for dictionary and list
  char **dic,**list;//contains dictionary/list
  char *filepath;//path from the file

  //check for arguments
  if (strlen(argv[1]) == 0){
    printf("Select list first!\n");
    exit(0);
  }

  //We load a dic as a pointer.
  dic_size = 0;
  dic_size = countDicWords(DICTIONARY);
  dic = getDictionary(DICTIONARY,dic_size);
  //load list
  filepath = createPath(DATABASE,argv[1]);
  list_size = countItems(filepath);
  list = getListItems(filepath,list_size);
  free(filepath);
  
  //start tree for every item in list
  for (int i = 0; i < list_size; i++){
    //creating path for file 
    filepath = createPath(DATABASE,list[i]);
    createTree(filepath,dic,dic_size,list_size,i+1,argv[2]);//create tree
    free(filepath);
  }

  //free memory
  deletepointers(dic,dic_size);
  deletepointers(list,list_size);

  return 0;
}

//Creates path for opening files
char * createPath(char *start,char *subpath){
  char *tmp = malloc(sizeof(char)*(strlen(start)+strlen(subpath)+1));
  strcpy(tmp,start);
  strcat(tmp,subpath);
  return tmp;
}

//Creates the tree of the file
void createTree(char *path,char **dic,int dic_size,int list_size,int count,char *mode){
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
  printf("Creating: %s Tree\n \n",path);

  //Insert dic to Tree
  for(int j = 0; j < dic_size; j++){
    a = dic[j];
    n_data = malloc(sizeof(node_data));
    n_data->key = a;
    n_data->num_times = 0;
    insert_node(tree, n_data);
  }
  //Increase dic words from file if they are in the tree.
  for (ct = 0; ct < *file_words; ct++) {
    a = file[ct];
    n_data = find_node(tree,a);

    if(n_data != NULL){
      n_data->num_times++;
    }
  }
  
  //We show the tree
  ct = 0;
  int i = 0;

  for(i = 0; i <= dic_size; i++)
  {
  	a = dic[i];
    n_data = find_node(tree, a);

    if (n_data && strcmp("all",mode) == 0) { 
      printf("La paraula %s apareix %d cops a l'arbre.\n", a,n_data->num_times);
      ct += n_data->num_times;
    }
    else if(n_data && strcmp("inserted",mode) == 0){
      if(n_data->num_times != 0){
        printf("La paraula %s apareix %d cops a l'arbre.\n", a,n_data->num_times);
        ct += n_data->num_times;
      }
    }
  }

  printf("\nNombre total que vegades que s'ha accedit a l'arbre: %d\n", ct);

  //delete tree
  deletepointers(file,*file_words);
  delete_tree(tree);
  free(tree);
  if(list_size == count){
    free(a);
  }
  free(n_data);
  free(file_words);
}
