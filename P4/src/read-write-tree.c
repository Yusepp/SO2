#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "read-write-tree.h"

#define MAGIC_NUMBER 0x01234567

/**
 *
 *  Count number of nodes of the tree. This is the recursive function.
 *  Do not call directly.
 *
 */

int count_nodes_recursive(node *x)
{
    int nodes;

    nodes = 0;

    /* Analyze the children */
    if (x->right != NIL)
        nodes += count_nodes_recursive(x->right);

    if (x->left != NIL)
        nodes += count_nodes_recursive(x->left);

    /* Take into account the node itself */
    nodes += 1;

    return nodes;
}

/**
 *
 *  Count the number of nodes in a tree 
 *
 */

int count_nodes(rb_tree *tree)
{
    int nodes;

    nodes = count_nodes_recursive(tree->root);

    return nodes;
} 

/**
 *
 *  Save node in disc. Main function is save_tree. 
 *
 */

void save_tree_recursive(node *x, FILE *fp)
{
    int len, num_times;

    if (x->right != NIL)
        save_tree_recursive(x->right, fp);

    if (x->left != NIL)
        save_tree_recursive(x->left, fp);

    /* Save length of key */
    len = strlen(x->data->key);
    fwrite(&len, sizeof(int), 1, fp);

    /* Save key */
    fwrite(x->data->key, len, sizeof(char), fp);

    /* Save number of times it has appeared */
    num_times = x->data->num_times;
    fwrite(&num_times, sizeof(int), 1, fp);
}

/**
 *
 *  Function that saves a tree on disc 
 *
 */

void save_tree(rb_tree *tree, char *filename)
{
    FILE *fp;

    int magic, nodes;

    fp = fopen(filename, "w");
    if (!fp) {
        printf("ERROR: no he pogut obrir el fitxer per escriptura.\n");
        return;
    }

    magic = MAGIC_NUMBER;
    fwrite(&magic, sizeof(int), 1, fp);

    nodes = count_nodes(tree);
    fwrite(&nodes, sizeof(int), 1, fp);

    save_tree_recursive(tree->root, fp);

    fclose(fp);  
}

/**
 *
 *  Load tree from disc
 *
 */

rb_tree *load_tree(char *filename)
{
    FILE *fp;
    rb_tree *tree;
    node_data *n_data;

    int i, len, magic, nodes, num_times;
    char *str;

    fp = fopen(filename, "r");
    if (!fp) {
        printf("Could not open file %s.\n", filename);
        return NULL;
    }

    /* We read the magic number */ 
    fread(&magic, sizeof(int), 1, fp);
    if (magic != MAGIC_NUMBER) {
        printf("Magic number is not correct.\n");
        return NULL;
    }

    /* Read the number of nodes of the tree */ 
    fread(&nodes, sizeof(int), 1, fp);

    /* Allocate memory for tree */
    tree = (rb_tree *) malloc(sizeof(rb_tree));

    /* Initialize the tree */
    init_tree(tree);

    /* Read data from disc */ 
    for(i = 0; i < nodes; i++)
    {
        /* Read length of word */
        fread(&len, sizeof(int), 1, fp);

        /* Read word */
        str = malloc(sizeof(char) * (len + 1)); 
        fread(str, sizeof(char), len, fp);

        /* Put and end-of-str at the word */
        str[len] = 0;

        /* Read number of times */
        fread(&num_times, sizeof(int), 1, fp);
        
        /* Allocate memory for the node */ 
        n_data = malloc(sizeof(node_data));

        n_data->key       = str;
        n_data->num_times = num_times;

        /* Insert node in the tree */ 
        insert_node(tree, n_data);
    }

    fclose(fp);

    return tree;
}

