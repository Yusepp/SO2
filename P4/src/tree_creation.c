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
#include <sys/mman.h>

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
#define NUM_PROC 4

typedef struct shared_mem {
    sem_t clau1;
    sem_t clau2;
} shared_mem;

shared_mem *s;




/*
*
*   CREATION OF THE TREE.
*
*
*/


rb_tree * createTree(char *pathdic,char *pathfile){

 int dic_size,list_size;//indexes for dictionary and list
 char **dic,**list,**file;;//contains dictionary/list/file
 char *filepath;//path from the file
 int *file_words = 0;//how many words in file
 char *mapped_tree,*mapped_names;//mapped variables
 FILE *folder;//folder file pointer

 //We load a dic as a pointer.
 dic_size = 0;
 filepath = createPath(DICTIONARY,pathdic);
 dic_size = countDicWords(filepath);
 dic = getDictionary(filepath,dic_size);
 list_size = countItems(filepath);
 

 free(filepath);

 rb_tree *tree;//tree
 node_data *n_data;//node

 /* Allocate memory for tree */
 tree = (rb_tree *) malloc(sizeof(rb_tree));
 /* Initialize the tree */
 init_tree(tree);
 indexDict(tree,dic,dic_size);//Dictionary to tree
 mapped_tree = serialize_node_data_to_mmap(tree);//mapping tree(serialize)

 filepath = createPath(DATABASE,pathfile);//path from list
 folder = fopen(filepath,"r");//open list
 mapped_names = dbfnames_to_mmap(folder);//mapping file's names

s = mmap(NULL, sizeof(shared_mem), PROT_READ | PROT_WRITE,
      MAP_SHARED | MAP_ANONYMOUS, -1, 0);


  int pid;
  sem_init(&s->clau1,1,1);
  sem_init(&s->clau2,1,1);

  for(int i = 0; i<NUM_PROC; i++){
    pid = fork();
    if(pid== 0){
		    for(int j=i; j<list_size;j+=NUM_PROC){
        //process_list(tree,mapped_names,i);//process list of files
          if(get_dbfname_from_mmap(mapped_names,j) != NULL){
            printf("Process %d processing %s\n",i+1,get_dbfname_from_mmap(mapped_names,j));
            filepath = createPath(DATABASE,get_dbfname_from_mmap(mapped_names,j));
            //sem_wait(&s->clau1);
		        process_file1(tree,filepath);
            //sem_post(&s->clau1);
          }
       }
        exit(0);
    }
  }
  for(int i = 0; i<NUM_PROC; i++){
      wait(NULL);
      printf("Process %d Finished!\n",i+1);
  }


/* Parte josep
for(int i = 0; i<NUM_PROC; i++){//We create N process
    if(fork() == 0){//Child process
      process_list(tree,mapped_names,i);//Child process file
      exit(0);//finishes
    }
    else{//parent
      printf("Process %d Created!\n",i);
    }
  }

  for(int i = 0; i<NUM_PROC; i++){
    wait(NULL);//parent works to every process
    printf("Process %d Finished!\n",i);
  }*/


  deserialize_node_data_from_mmap(tree,mapped_tree);//unmapping
  dbfnames_munmmap(mapped_names);//unmapping
  sem_close(&s->clau1);//closing semaphore
  sem_close(&s->clau2);//closing semaphore
  munmap(s,sizeof(shared_mem));
  return tree;
}


/*
*
*	COPY DIC WORDS TO TREE.
*
*
*/
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

/*
*
*	PROCESS A LIST OF FILES FROM THE DATABASE
*
*
*/
void process_list(rb_tree *tree,char *mapped_names,int process){

  int i = 0;
  while(get_dbfname_from_mmap(mapped_names,i) != NULL){
    printf("`Process %d : %s\n",process,get_dbfname_from_mmap(mapped_names,i));
    char *filepath = createPath(DATABASE,get_dbfname_from_mmap(mapped_names,i));
    int *file_words = malloc(sizeof(int));
    char **file = process_file(filepath,file_words);
    //Increase dic words from file if they are in the tree.
    sem_wait(&s->clau1);
    indexFile(tree,file,*file_words);
    sem_post(&s->clau1);
    deletepointers(file,*file_words);
    i++;
  }
}



/*
*
*	COPY WORDS FORM A FILE TO THE TREE
*
*
*/

void indexFile(rb_tree *tree,char **file,int size){
  node_data *n_data;//node
  for (int ct = 0; ct < size; ct++) {
    n_data = find_node(tree,file[ct]);

    if(n_data != NULL){
         n_data->num_times++;
    }
  }
}

/*
*
*	GIVEN A PATH CREATE THE DATABASE OR DIC PATH.
*
*
*/

char * createPath(char *start,char *subpath){
  char *tmp = malloc(sizeof(char)*(strlen(start)+strlen(subpath)+1));
  strcpy(tmp,start);
  strcat(tmp,subpath);
  return tmp;
}

/*
*
* FUNCIONES QUE USO PARA PROBAR MI PARTE DEL CODIGO
*
*/

void process_file1(rb_tree *tree, char *fname)
{
  FILE *fp;
  char line[MAXCHAR];

  fp = fopen(fname, "r");
  if (!fp) {
    printf("Could not open %s\n", fname);
    exit(1);
  }

  while (fgets(line, MAXCHAR, fp))
    index_words_line(tree, line);

  fclose(fp);
}



void index_words_line(rb_tree *tree, char *line)
{
  node_data *n_data;

  int i, j, is_word, len_line;
  char paraula[MAXCHAR];

  i = 0;

  len_line = strlen(line);

  /* Search for the beginning of a candidate word */

  while ((i < len_line) && (isspace(line[i]) || ((ispunct(line[i])) && (line[i] != '#')))) i++;

  /* This is the main loop that extracts all the words */

  while (i < len_line)
  {
    j = 0;
    is_word = 1;

    /* Extract the candidate word including digits if they are present */

    do {

      if ((isalpha(line[i])) || (line[i] == '\''))
        paraula[j] = line[i];
      else
        is_word = 0;

      j++; i++;

      /* Check if we arrive to an end of word: space or punctuation character */

    } while ((i < len_line) && (!isspace(line[i])) && (!(ispunct(line[i]) && (line[i]!='\'') && (line[i]!='#'))));

    /* If word insert in list */

    if (is_word) {

      /* Put a '\0' (end-of-word) at the end of the string*/
      paraula[j] = 0;

      /* Search for the word in the tree */
      n_data = find_node(tree, paraula);

      if (n_data != NULL){
		      sem_wait(&s->clau2);
          n_data->num_times++;
		      sem_post(&s->clau2);
	       }
    }

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || ((ispunct(line[i])) && (line[i] != '#')))) i++;

  } /* while (i < len_line) */
}
