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

#include "red-black-tree.h"

#define MAXVALUE 10

/**
 *
 *  Main function. This function is an example that shows
 *  how the binary tree works.
 *
 */

int main(int argc, char **argv)
{
  int a, maxnum, ct;

  rb_tree *tree;
  node_data *n_data;

  if (argc != 2)
  {
    printf("Usage: %s maxnum\n", argv[0]);
    exit(1);
  }

  maxnum = atoi(argv[1]);

  printf("Test with red-black-tree\n");

  /* Random seed */
  srand(time(NULL));

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));

  /* Initialize the tree */
  init_tree(tree);

  for (ct = 0; ct < maxnum; ct++) {
    /* Generate random key to be inserted in the tree */
    a = rand() % MAXVALUE + 1;

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

  for(a = 1; a <= MAXVALUE; a++)
  {
    n_data = find_node(tree, a);

    if (n_data) { 
      printf("El numero %d apareix %d cops a l'arbre.\n", a, n_data->num_times);
      ct += n_data->num_times;
    }
  }

  printf("Nombre total que vegades que s'ha accedit a l'arbre: %d\n", ct);
  
  /* Delete the tree */
  delete_tree(tree);
  free(tree);

  return 0;
}

