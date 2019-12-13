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



#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/"
#define NUM_PROC  2 

/*
*
*	Search recursively the word with most ocurrences.
*
*
*/
node_data * topWord(rb_tree * tree){
  if (tree->root != NIL)
    return recursive_search(tree->root,tree->root)->data;
}

/*
*
*	Aux Method for search.
*
*
*/
node *recursive_search(node *n,node *best){
  node *tmp = best;
  if(n->data->num_times > best->data->num_times){
    tmp = n;
  }

  if (n->right != NIL){
    tmp = recursive_search(n->right,tmp);
  }

  if (n->left != NIL){
    tmp = recursive_search(n->left,tmp);
  }

  return tmp;
}

/*
*
*	Main search method.
*
*
*/
void search_word(char *str1,rb_tree *tree){
  if(strlen(str1) == 1){
    node_data *n_data = topWord(tree);
    printf("La paraula %s es la que mes apareix: %d vegades a l'arbre.\n", n_data->key, n_data->num_times);
  }
  else{
    str1[strlen(str1)-1]=0;
    node_data *n_data = find_node(tree, str1);
    if(n_data)
      printf("La paraula %s apareix %d vegades a l'arbre.\n", str1, n_data->num_times);
    else
      printf("La paraula no apareix a l'arbre");
  }
}
