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
  tree->size = dic_size;
  //Increase dic words from file if they are in the tree.
  for (ct = 0; ct < *file_words; ct++) {
    n_data = find_node(tree,file[ct]);

    if(n_data != NULL){
      n_data->num_times++;
    }
  }
  
  //delete
  deletepointers(file,*file_words);
  free(n_data);
  free(file_words);
  free(filepath);

  return tree;
}

char * createPath(char *start,char *subpath){
  char *tmp = malloc(sizeof(char)*(strlen(start)+strlen(subpath)+1));
  strcpy(tmp,start);
  strcat(tmp,subpath);
  return tmp;
}

node_data * topWord(rb_tree * tree){
  if (tree->root != NIL)
    return recursive_search(tree->root,tree->root)->data;
}

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

void writeTree(FILE *fp,rb_tree * tree,int magicNumber){
  int magic = magicNumber;
  //Escribim el magicNumber a l'inici del fitxer
  fwrite(&magic, sizeof(int), 1, fp);

  //Escriu el numero de nodes a l'arbre

  fwrite(&(tree->size), sizeof(int),1,fp);

  //Escriu l'arbre a memoria
  writeTreeInicial(tree, fp);
  fclose(fp);
}
