#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>

#include "red-black-tree.h"
#include "read_tools.h"
#include "tree_operations.h"

#include "tree-to-mmap.h"
#include "dbfnames-mmap.h"

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
  char *mapped;
  FILE* fp;

  //We load a dic as a pointer.
  dic_size = 0;
  filepath = createPath(DICTIONARY,pathdic);
  dic_size = countDicWords(filepath);
  dic = getDictionary(filepath,dic_size);
  free(filepath);
  //load list
  filepath = createPath(DATABASE,pathfile);
  list_size = countItems(filepath);
  list = getListItems(filepath,list_size);
  free(filepath);

  rb_tree *tree;//tree
  node_data *n_data;//node

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(tree);
  indexDict(tree,dic,dic_size);
  mapped = serialize_node_data_to_mmap(tree);

  process_list(tree,list,list_size);//process list of files

  deserialize_node_data_from_mmap(tree,mapped);


  return tree;
}


rb_tree * createTreeTest(char *pathdic,char *pathfile){

  int dic_size,list_size;//indexes for dictionary and list
  char **dic,**list;//contains dictionary/list
  char *filepath;//path from the file
  int *file_words = 0;//how many words in file
  char **file;//contains words in file
  int  ct;//counter of nodes
  char *mapped;
  FILE* fp;
  int i;
  char **files;

  //We load a dic as a pointer.
  dic_size = 0;
  filepath = createPath(DICTIONARY,pathdic);
  dic_size = countDicWords(filepath);
  dic = getDictionary(filepath,dic_size);
  free(filepath);
  //load list
  filepath = createPath(DATABASE,pathfile);
  list_size = countItems(filepath);
  list = getListItems(filepath,list_size);
  free(filepath);

  /*Mapeja els noms dels fitxers a memoria*/
  fp= fopen(filepath,"r");
  dbfnames_to_mmap(fp);
  printf("%s\n", dbfnames_to_mmap(fp));
  
  for (i = 0; i < list_size; i++) {

  }
  rb_tree *tree;//tree
  node_data *n_data;//node

  /* Allocate memory for tree */
  tree = (rb_tree *) malloc(sizeof(rb_tree));
  /* Initialize the tree */
  init_tree(tree);
  indexDict(tree,dic,dic_size);

  mapped = serialize_node_data_to_mmap(tree);

  process_list(tree,list,list_size);//process list of files

  deserialize_node_data_from_mmap(tree,mapped);


  return tree;
}

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
void process_list(rb_tree *tree,char **list,int size){
  for (int i = 0; i < size; i++){
    //creating path for file
    char *filepath = createPath(DATABASE,list[i]);
    //We load a file as a pointer.
    int *file_words = malloc(sizeof(int));
    char **file = process_file(filepath,file_words);
    //Increase dic words from file if they are in the tree.
    indexFile(tree,file,*file_words);
    deletepointers(file,*file_words);
    free(file_words);
    free(filepath);
  }
}

void indexFile(rb_tree *tree,char **file,int size){
  node_data *n_data;//node
  for (int ct = 0; ct < size; ct++) {
    n_data = find_node(tree,file[ct]);

    if(n_data != NULL){
      n_data->num_times++;
    }
  }
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


/*
*
*	METODOS PARA INSERIR EN EL ARBOL Y CONTAR RECURSIVAMENTE
*
*
*/

/*
*Escriu les dades del node al fitxer, tant la key com el numero de vegades que apareix
*/
void writeTreeData(node_data *n_data, FILE *fp){
  int size = strlen(n_data->key);
  fwrite(&size, sizeof(int), 1, fp);
  fwrite(n_data->key, sizeof(char), strlen(n_data->key), fp);
  fwrite(&n_data->num_times, sizeof(int), 1, fp);
}


/*
*Metode que recorre l'arbre en inordre per poder escriure les paraules al fitxer
*/
void writeTreeInordre(node *x, FILE *fp){
  if(x->left != NIL)
    writeTreeInordre(x->left, fp);
  writeTreeData(x->data, fp);
  if(x->right != NIL)
    writeTreeInordre(x->right, fp);
}

/*
*Metode que començara a escriure a l'arbre desde el node inicial.
* Comprovara que tinguem un arbre i anira al metode d'inserir en inordre
*/
void writeTreeInicial(rb_tree *tree, FILE *fp){
  if(tree->root != NIL)
    writeTreeInordre(tree->root, fp);
}

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

