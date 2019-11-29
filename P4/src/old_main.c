/**
 *
 * Practica 3 
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "red-black-tree.h"
#include "create-tree.h"
#include "read-write-tree.h"
#include "consult-tree.h"

#define MAXCHAR      100


/**
 * 
 *  Menu
 * 
 */
int menu() 
{
  char str[5];
  int opcio;

  printf("\n\nMenu\n\n");
  printf(" 1 - Creacio de l'arbre\n");
  printf(" 2 - Emmagatzemar arbre a disc\n");
  printf(" 3 - Llegir arbre de disc\n");
  printf(" 4 - Consultar informacio de l'arbre\n");
  printf(" 5 - Sortir\n\n");
  printf("   Escull opcio: ");

  fgets(str, 5, stdin);
  opcio = atoi(str); 

  return opcio;
}

/**
 * 
 *  Main procedure
 *
 */

int main(int argc, char **argv)
{
  rb_tree *tree;

  char str1[MAXCHAR], str2[MAXCHAR];
  int opcio;

  tree = NULL;

  if (argc != 1)
    printf("Opcions de la linia de comandes ignorades\n");

  do {
    opcio = menu();
    printf("\n\n");

    /* Feu servir aquest codi com a pantilla */

    switch (opcio) {
      case 1:
        if (tree) {
          printf("Alliberant arbre actual.\n");
          delete_tree(tree);
          free(tree);
        }

        printf("Fitxer de diccionari de paraules: ");
        fgets(str1, MAXCHAR, stdin);
        str1[strlen(str1)-1]=0;

        printf("Fitxer de base de dades: ");
        fgets(str2, MAXCHAR, stdin);
        str2[strlen(str2)-1]=0;

        tree = create_tree(str1, str2);

        break;

      case 2:
        if (!tree) {
          printf("No hi ha cap arbre creat!\n");
          break;
        }

        printf("Nom de fitxer en que es desara l'arbre: ");
        fgets(str1, MAXCHAR, stdin);
        str1[strlen(str1)-1]=0;

        save_tree(tree, str1);

        break;

      case 3:
        if (tree)
        {
          delete_tree(tree);
          free(tree);
        }

        printf("Nom del fitxer que conte l'arbre: ");
        fgets(str1, MAXCHAR, stdin);
        str1[strlen(str1)-1]=0;

        tree = load_tree(str1);

        break;

      case 4:
        if  (!tree) {
          printf("L'arbre no ha sigut creat.\n");
          break;
        }

        printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
        fgets(str1, MAXCHAR, stdin);
        str1[strlen(str1)-1]=0;

        if (strlen(str1) == 0)
          consult_max_numtimes(tree);
        else
          consult_word_numtimes(tree, str1);

        break;

      case 5:

        if (tree) {
          delete_tree(tree);
          free(tree);
        }

        break;

      default:
        printf("Opcio no valida\n");

    } /* switch */
  }
  while (opcio != 5);

  return 0;
}

