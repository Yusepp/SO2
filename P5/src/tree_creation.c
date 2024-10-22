#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>
#include <sys/mman.h>
#include <pthread.h>

#include "red-black-tree.h"
#include "read_tools.h"
#include "tree_creation.h"
#include "search_operations.h"
#include "write_read.h"
#include <time.h>


#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/"
#define NUM_THREAD 2
int s = 1;
int idx = -1;

rb_tree *tree;//tree
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex2 = PTHREAD_MUTEX_INITIALIZER;
pthread_mutex_t mutex3 = PTHREAD_MUTEX_INITIALIZER;
int err;

struct args{
  int num_thread;
  rb_tree *local_tree;
  char **files;
  int files_size;
};


rb_tree * createTree(char *pathdic,char *pathfile){
  int dic_size,list_size;//indexes for dictionary and list
  char **dic,**list,**file;;//contains dictionary/list/file
  char *filepath;//path from the file
  int *file_words = 0;//how many words in file
  pthread_t ntid[NUM_THREAD];
  struct args *arg;
  FILE *files1;
  clock_t t;



  rb_tree *local_tree1;//tree

  //We load a dic as a pointer.
  dic_size = 0;
  filepath = createPath(DICTIONARY,pathdic);
  dic_size = countDicWords(filepath);
  dic = getDictionary(filepath,dic_size);

  free(filepath);



  node_data *n_data;//node

   /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Allocate memory for tree */
  local_tree1 = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(tree);
  indexDict(tree,dic,dic_size);//Dictionary to tree
  init_tree(local_tree1);
  indexDict(local_tree1,dic,dic_size);//Dictionary to tree
  filepath = createPath(DATABASE,pathfile);//path from list
  /*Getting file names*/
  files1 = fopen(filepath,"r");
  free(filepath);

  if(files1 == NULL){
    printf("Error getting filenames\n");
    exit(1);
  }

  list = getFiles(files1,&list_size);
  fclose(files1);
  t = clock();
  for(int i = 0; i<NUM_THREAD; i++){
    arg = malloc(sizeof(struct args));
    arg->num_thread = i;
    arg->local_tree = local_tree1;
    arg->files = list;
    arg->files_size = list_size;
    printf("Thread %d created\n",i+1);
    err = pthread_create(&(ntid[i]), NULL, thread_fn, (void *) arg);
    if(err != 0){
      printf("Error creating thread\n");
      exit(1);
    }

  }

  for(int i = 0; i<NUM_THREAD; i++){
    err = pthread_join(ntid[i], NULL);
    if(err != 0){
      printf("Error waiting thread\n");
      exit(1);
    }
    printf("Thread %d finished\n",i+1);
  }

  t = clock() - t;
  double time_taken = ((double)t)/CLOCKS_PER_SEC; // in seconds
  printf("Execution time: %f s",time_taken);
  delete_tree(local_tree1);
  free(local_tree1);
  free(arg);
  free(filepath);

  return tree;
}


/*
*
*	COPY DIC WORDS TO TREE.
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
*	GIVEN A PATH CREATE THE DATABASE OR DIC PATH.
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
 * GIVEN A FILE PROCESS LINES OF FILE
 *
 */
char **getFiles(FILE *fp_db,int *size){
  char ** files;
  char line[MAXCHAR];

  fgets(line, MAXCHAR, fp_db);
  *size = atoi(line);
  if (size <= 0) {
    printf("Number of files is %d\n", *size);
    exit(1);
  }

  files = malloc(sizeof(char *)*(*size));
  if(files == NULL){
    printf("Error allocating filenames\n");
    exit(1);
  }

  for(int i = 0; i< *size; i++){
    files[i] = malloc(sizeof(char)*MAXCHAR);
  }

  /* Read database files */
  for(int i = 0; i < *size; i++) {
    fgets(files[i], MAXCHAR, fp_db);
    /* Remove '\n' from line */
    files[i][strlen(files[i])-1] = 0;
  }

  return files;
}

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

void *thread_fn(void *par){
  /*Cast of arguments to our struct*/
  printf("Accesing thread func\n");
  struct args *arg = (struct args *)par;
  /*Process files assigned to this thread*/
  while(idx < arg->files_size-1){

    pthread_mutex_lock(&mutex1);
    idx++;
    char *pathfile = createPath(DATABASE,arg->files[idx]);
    printf("Thread %d : %s\n",arg->num_thread+1,pathfile);
    pthread_mutex_unlock(&mutex1);
    process_file1(arg->local_tree,pathfile);
  }
  localToGlobal(tree,arg->local_tree);



}
/*
*
* AUXILIARY METHOD THAT CALLS THE RECURSIVE FUNCTION TO COPY THE NODES
* OF THE LOCAL TREE TO THE GLOBAL
*
*/

void localToGlobal(rb_tree *global,rb_tree *local){
  if(global->root != NIL && local->root != NIL){
    copyRecursive(global->root,local->root);
  }
}

/*
*
* RECURSIVE METHOD THAT COPIES THE NODES OF THE LOCAL TREE TO THE GLOBAL
*
*/
void copyRecursive(node *global,node *local){
  if(global != NIL && local != NIL){
    pthread_mutex_lock(&mutex2);
    global->data->num_times = local->data->num_times;
    pthread_mutex_unlock(&mutex2);
    copyRecursive(global->left,local->left);
    copyRecursive(global->right,local->right);
  }
}


/*
 *
 * GIVEN A LINE WITH WORDS, SEARCH EACH WORD IN THE TREE AND INCRESE IF IT IS FOUND
 *
 */

void index_words_line(rb_tree *tree, char *line){
  node_data *n_data;

  int i, j, is_word, len_line;
  char paraula[MAXCHAR];

  i = 0;

  len_line = strlen(line);

  /* Search for the beginning of a candidate word */
  while ((i < len_line) && (isspace(line[i]) || ((ispunct(line[i])) && (line[i] != '#')))) i++;

  /* This is the main loop that extracts all the words */
  while (i < len_line)  {
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
          pthread_mutex_lock(&mutex3);
          n_data->num_times++;
          pthread_mutex_unlock(&mutex3);

      }
    }

    /* Search for the beginning of a candidate word */
    while ((i < len_line) && (isspace(line[i]) || ((ispunct(line[i])) && (line[i] != '#')))) i++;

  }
}
