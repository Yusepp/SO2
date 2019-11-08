#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"
#include "read_tools.h"
#include "tree_operations.h"

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/"

rb_tree * createTree(char *pathdic,char *pathfile){

  int dic_size,list_size;//indexes for dictionary and list
  char **dic,**list;//contains dictionary/list
  char *filepath;//path from the file
  int *file_words = 0;//how many words in file
  char **file;//contains words in file
  int  ct;//counter of nodes

  //We load a dic as a pointer.
  dic_size = 0;
  filepath = createPath(DICTIONARY,pathdic);
  dic_size = countDicWords(filepath);
  dic = getDictionary(filepath,dic_size);
  free(filepath);
  //load list
  filepath = createPath(DATABASE,pathfile);
  //list_size = countItems(filepath);
  //list = getListItems(filepath,list_size);
  
  //We load a file as a pointer.
  file_words = malloc(sizeof(int));
  file = process_file(filepath,file_words);

  rb_tree *tree;//tree
  node_data *n_data;//node

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(tree);

  //Insert dic to Tree
  for(int j = 0; j < dic_size; j++){
    n_data = malloc(sizeof(node_data));
    n_data->key = dic[j];
    n_data->num_times = 0;
    insert_node(tree, n_data);
  }
  //Increase dic words from file if they are in the tree.
  for (ct = 0; ct < *file_words; ct++) {
    n_data = find_node(tree,file[ct]);

    if(n_data != NULL){
      n_data->num_times++;
    }
  }

  //We show the tree
  /*ct = 0;
  int i = 0;

  for(i = 0; i <= dic_size; i++){
  	a = dic[i];
    n_data = find_node(tree, a);
    if(n_data->num_times != 0){
      printf("La paraula %s apareix %d cops a l'arbre.\n", a,n_data->num_times);
      ct += n_data->num_times;
    }
  }*/
  
  //delete
  deletepointers(file,*file_words);
  free(n_data);
  free(file_words);
  free(filepath);
  //deletepointers(list,list_size);

  return tree;
}

char * createPath(char *start,char *subpath){
  char *tmp = malloc(sizeof(char)*(strlen(start)+strlen(subpath)+1));
  strcpy(tmp,start);
  strcat(tmp,subpath);
  return tmp;
}
