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
#include "tree_operations.h"
#include "read_tools.h"
#define MAXCHAR      100
#define MAGIC_NUMBER 0x01234567



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
    char str1[MAXCHAR], str2[MAXCHAR];
    int opcio;
    FILE *fp;
    rb_tree *tree;
    node_data *n_data;

    if (argc != 1)
        printf("Opcions de la linia de comandes ignorades\n");

    do {
        opcio = menu();
        printf("\n\n");

       /* Feu servir aquest codi com a pantilla */

        switch (opcio) {
            case 1:
                if(tree){//deleting old tree
                  delete_tree(tree);
                  free(tree);
                }
                printf("Fitxer de diccionari de paraules: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                printf("Fitxer de base de dades: ");
                fgets(str2, MAXCHAR, stdin);
                str2[strlen(str2)-1]=0;

                tree = createTree(str1,str2);

                break;

            case 2:
                if (!tree) {
                  printf("No hi ha cap arbre creat!\n");
                  break;
                }

                printf("Nom de fitxer en que es desara l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                fp = fopen(str1, "w");

                if(!fp){
                  printf("No es pot guardar el fitxer\n");
                  exit(0);
                }
                writeTree(fp,tree,MAGIC_NUMBER);
                fclose(fp);
                break;

            case 3:

                if(tree){//deleting old tree
                  delete_tree(tree);
                  free(tree);
                }

                printf("Nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;
                fp = fopen(str1, "r");
                if(!fp){
                  printf("No es pot obrir el fitxer\n");
                  exit(0);
                }
                tree = readTree(fp,MAGIC_NUMBER);
                fclose(fp);
                break;
            case 4:
                printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAXCHAR, stdin);
                search_word(str1,tree);
                break;

            case 5:
                //Eliminem l'abre que esta guardat a memoria y alliberem espai.
                if(tree){
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



