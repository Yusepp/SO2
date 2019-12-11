#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <semaphore.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <pthread.h>

#include "red-black-tree.h"
#include "read_tools.h"
#include "tree_creation.h"
#include "search_operations.h"
#include "write_read.h"

#include "tree-to-mmap.h"
#include "dbfnames-mmap.h"

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/"
#define NUM_PROC  2 


sem_t clau;

rb_tree * createTree(char *pathdic,char *pathfile){

  int dic_size,list_size;//indexes for dictionary and list
  char **dic,**list;//contains dictionary/list
  char *filepath;//path from the file
  int *file_words = 0;//how many words in file
  char **file;//contains words in file
  int  ct;//counter of nodes
  char *mapped_tree;
  char *mapped_names;
  FILE *folder;

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


  rb_tree *tree;//tree
  node_data *n_data;//node

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(tree);
  indexDict(tree,dic,dic_size);//Dictionary to tree
  mapped_tree = serialize_node_data_to_mmap(tree);//mapping tree(serialize)

  folder = fopen(filepath,"r");
  mapped_names = dbfnames_to_mmap(folder);//mapping file's names

  sem_init(&clau,0,1);

  for(int i = 0; i<NUM_PROC; i++){
    if(fork() == 0){
      process_list(tree,mapped_names,i);//process list of files
      exit(0);
    }
    else{
      printf("Process %d Created!\n",i);
    }
  }

  for(int i = 0; i<NUM_PROC; i++){
    wait(NULL);
    printf("Process %d Finished!\n",i);
  }

  deserialize_node_data_from_mmap(tree,mapped_tree);//unmapping
  dbfnames_munmmap(mapped_names);//unmapping
  sem_close(&clau);

  return tree;
}



void indexDict(rb_tree *tree,char **dic,int size){
  //Insert dic to Tree
  node_data *n_data;//node
  for(int j = 0; j < size; j++){
    n_data = malloc(sizeof(node_data));
    n_data->key = dic[j];
    n_data->num_times = 0;
    insert_node(tree, n_data);
  }
  tree->size = size;
}
void process_list(rb_tree *tree,char *mapped_names,int process){

  int i = 0;
  while(get_dbfname_from_mmap(mapped_names,i) != NULL){
    printf("`Process %d : %s\n",process,get_dbfname_from_mmap(mapped_names,i));
    char *filepath = createPath(DATABASE,get_dbfname_from_mmap(mapped_names,i));
    int *file_words = malloc(sizeof(int));
    char **file = process_file(filepath,file_words);
    //Increase dic words from file if they are in the tree.
    sem_wait(&clau);
    indexFile(tree,file,*file_words);
    sem_post(&clau);
    deletepointers(file,*file_words);
    i++;
  }
}

void indexFile(rb_tree *tree,char **file,int size){
  node_data *n_data;//node
  for (int ct = 0; ct < size; ct++) {
    n_data = find_node(tree,file[ct]);

    if(n_data != NULL){
      n_data->num_times++;
    }
  }
}

char * createPath(char *start,char *subpath){
  char *tmp = malloc(sizeof(char)*(strlen(start)+strlen(subpath)+1));
  strcpy(tmp,start);
  strcat(tmp,subpath);
  return tmp;
}


