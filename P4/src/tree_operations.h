/**
 *  
 * EASY TREE CREATION
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"
#include "read_tools.h"

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/"


rb_tree * createTree(char *pathdic,char *pathfile);//creates tree
void indexDict(rb_tree *tree,char **dic,int size);
void process_list(rb_tree *tree,char **list,int size);
void indexFile(rb_tree *tree,char **file,int size);
char * createPath(char *start,char *subpath);//creation path
node_data * topWord(rb_tree * tree);//return the top word of the tree
node *recursive_search(node *n,node *best);//recursive
void writeTree(FILE *fp,rb_tree * tree,int magicNumber);
rb_tree *readTree(FILE * fp,int magicNumber);
void writeTreeData(node_data *n_data, FILE *fp);
void writeTreeInordre(node *x, FILE *fp);
void writeTreeInicial(rb_tree *tree, FILE *fp);
void search_word(char *str1,rb_tree *tree);