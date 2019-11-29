#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "consult-tree.h"

/**
 *  
 *  Get the word that appears most. Recursive part, see consult_max_numtimes 
 *
 */

void consult_max_numtimes_recursive(node *x, char **word, int *num_times)
{
    node_data *n_data;

    /* Get node data */
    n_data = x->data;

    /* Check num_times and compare it with the maximum we currently have */
    if (n_data->num_times > *num_times) {
      *num_times = n_data->num_times;
      *word = n_data->key;
    }

    if (x->right != NIL)
        consult_max_numtimes_recursive(x->right, word, num_times);

    if (x->left != NIL)
        consult_max_numtimes_recursive(x->left, word, num_times);
}

/**
 *
 *  Get the word that appears most. 
 *
 */

void consult_max_numtimes(rb_tree *tree)
{
    char *word;
    int num_times;

    word = NULL;
    num_times = 0;

    if (tree->root != NIL)
        consult_max_numtimes_recursive(tree->root, &word, &num_times);

    printf("La paraula que apareix mes vegades es: %s (%d vegades)\n", word, num_times);
}


/**
 *
 *  Get the number of times a word appears. 
 *
 */

void consult_word_numtimes(rb_tree *tree, char *word)
{
  node_data *n_data;

  n_data = find_node(tree, word);

  if (n_data != NULL)
    printf("La paraula %s apareix %d vegades.\n", word, n_data->num_times);
  else
    printf("La paraula %s no apareix a l'arbre.\n", word);
}




