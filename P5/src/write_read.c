
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


/*
*
*	We read a binary in a concrete format to recover a saved tree.
*
*
*/
rb_tree *readTree(FILE * fp,int magicNumber){
  //Variables
  int tmp,numkeys,length;
  char * word;
  node_data *n_data;//node for insert
  rb_tree *tree;


  //creating new tree
  tree = (rb_tree *)malloc(sizeof(rb_tree));
  init_tree(tree);


  fread(&tmp,sizeof(int),1,fp);//read magicnumber


  if(tmp == magicNumber){//check that we read the correct magic
    fread(&tmp, sizeof(int), 1 , fp);//read size of tree

    tree->size = tmp;//new tree size
    for(int i = 0; i < tmp;i++){//make size times the node structure read

      fread(&length,sizeof(int),1,fp);//length of the node key

      //Reservem memoria per la paraula

      word = (char *) malloc(sizeof(char) * (length+1));//key + 1(for 0 byte)
      fread(word,sizeof(char),length,fp);//reading the key
      word[length] = 0;//insert byte 0

      fread(&numkeys,sizeof(int),1,fp);//value of the node

      n_data = malloc(sizeof(node_data));//reserve memory for node
      n_data->key = word;//putting key
      n_data->num_times = numkeys;//putting value

      printf("%s\n",n_data->key);//showing word read

      insert_node(tree,n_data);//inserting
    }
  }

  return tree;
}

/*
*
*	We save a binary in a concrete format.
*
*
*/
void writeTree(FILE *fp,rb_tree * tree,int magicNumber){
  int magic = magicNumber;
  //Escribim el magicNumber a l'inici del fitxer
  fwrite(&magic, sizeof(int), 1, fp);

  //Escriu el numero de nodes a l'arbre
  fwrite(&(tree->size), sizeof(int),1,fp);

  //Escriu l'arbre a memoria
  writeTreeInicial(tree, fp);
}


/*
*
*	Writes key length, key and ocurrence.
*
*
*/
void writeTreeData(node_data *n_data, FILE *fp){
  int size = strlen(n_data->key);
  fwrite(&size, sizeof(int), 1, fp);
  fwrite(n_data->key, sizeof(char), strlen(n_data->key), fp);
  fwrite(&n_data->num_times, sizeof(int), 1, fp);
}


/*
*
*	Moves through tree.
*
*
*/
void writeTreeInordre(node *x, FILE *fp){
  if(x->left != NIL)
    writeTreeInordre(x->left, fp);
  writeTreeData(x->data, fp);
  if(x->right != NIL)
    writeTreeInordre(x->right, fp);
}

/*
*
*	Starting write method.
*
*
*/
void writeTreeInicial(rb_tree *tree, FILE *fp){
  if(tree->root != NIL)
    writeTreeInordre(tree->root, fp);
}
