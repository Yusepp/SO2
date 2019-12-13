#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#include "red-black-tree.h"

/**
 *
 *  Count memory that is occupied by a node data. 
 *  Do not call directly.
 *
 */

int get_node_data_memory_recursive(node *x)
{
    int memory;

    memory = 0;

    /* Analyze the children */
    if (x->right != NIL)
        memory += get_node_data_memory_recursive(x->right);

    if (x->left != NIL)
        memory += get_node_data_memory_recursive(x->left);

    /* Take into account the node itself */
    memory += sizeof(node_data); 

    return memory;
}

/**
 *
 *  Count the memory that the data of nodes occupy
 *  (we do not count all the remaining memory the
 *  tree occupies)
 *
 */

int get_node_data_memory(rb_tree *tree)
{
    int memory;

    memory = get_node_data_memory_recursive(tree->root);

    return memory;
}

/**
 *
 *  Serialize the node data of a tree to a mmap.
 *  This is the recursive function. Do not call directly.
 *
 */

char *serialize_node_data_to_mmap_recursive(node *x, char *pointer_mmap)
{
  node_data *n_data_ori, *n_data_dest;

  /* Analyze the children */
  if (x->right != NIL)
    pointer_mmap = serialize_node_data_to_mmap_recursive(x->right, pointer_mmap);

  if (x->left != NIL)
    pointer_mmap = serialize_node_data_to_mmap_recursive(x->left, pointer_mmap);

  /* Here comes the tricky thing. Variable 'pointer_mmap' is associated
   * to the mmap. Data of the tree will be stored here. The original
   * data will be freed. */

  /* Get the original data */

  n_data_ori = x->data;

  /* At the mmap, here will be the node stored. */

  n_data_dest = (node_data *) pointer_mmap;

  /* Update the data of n_data_dest */

  n_data_dest->key       = n_data_ori->key;
  n_data_dest->num_times = n_data_ori->num_times;

  /* Free the original node data */

  free(n_data_ori);

  /* Assign the new serialized data to the node of the tree */

  x->data = n_data_dest;

  /* Increment pointer_mmap so that it points to the next free memory
   * space of the mmap. */

  pointer_mmap += sizeof(node_data); 

  /* And return the pointer... did you get what we have done? */

  return pointer_mmap;
}

/**
 *
 *  Convert a tree by serializing node data to a mmap. 
 *
 */

char *serialize_node_data_to_mmap(rb_tree *tree)
{
  int node_data_memory, total_memory, *p_node_data_memory;
  char *mmap_node_data, *pointer;

  /* Get the memory of the data nodes */

  node_data_memory = get_node_data_memory(tree);
  total_memory = node_data_memory + sizeof(int);

  /* Here will be node data mmaped. We map for writing and reading! */

  mmap_node_data = mmap(0, total_memory, PROT_WRITE | PROT_READ, 
      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (mmap_node_data == MAP_FAILED) {
    printf("Map failed. Errno = %d\n", errno);
    exit(1);
  }

  /* Store the size at the beggining of the mapping */

  pointer = mmap_node_data;

  p_node_data_memory = (int *) mmap_node_data;
  *p_node_data_memory = total_memory;

  /* Let us now serialize the data of the tree */

  pointer += sizeof(int);
  serialize_node_data_to_mmap_recursive(tree->root, pointer);

  /* Return mmap */

  return mmap_node_data;
}


/**
 *
 *   Deserialize node data from a mmap. Recursive function.
 *
 */

void deserialize_node_data_from_mmap_recursive(node *x)
{
    node_data *n_data_ori, *n_data_dest;

    /* Analyze the children */
    if (x->right != NIL)
        deserialize_node_data_from_mmap_recursive(x->right);

    if (x->left != NIL)
        deserialize_node_data_from_mmap_recursive(x->left);

    /* At the mmap, here is the node data stored. */

    n_data_ori = x->data; 

    /* Allocate memory for node data */ 

    n_data_dest = malloc(sizeof(node_data));

    /* Copy data to the new location */

    n_data_dest->key       = n_data_ori->key;
    n_data_dest->num_times = n_data_ori->num_times;

    /* Assign to the node of the tree */

    x->data = n_data_dest;
}

/**
 *
 *  Deserealize data from a mmap. Main function. This funtion also
 *  performs the unmmapping.
 *
 */

void deserialize_node_data_from_mmap(rb_tree *tree, char *mmap_node_data)
{
    int total_memory, *p_total_memory;

    /* Deserealization */

    deserialize_node_data_from_mmap_recursive(tree->root);

    /* Get the total memory associated to the mapping */

    p_total_memory = (int *) mmap_node_data;
    total_memory = *p_total_memory;

    /* Unmap */

    munmap(mmap_node_data, total_memory);
}

