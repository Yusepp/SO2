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
    int opcio, magicNumber;
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
                printf("Fitxer de diccionari de paraules: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                printf("Fitxer de base de dades: ");
                fgets(str2, MAXCHAR, stdin);
                str2[strlen(str2)-1]=0;

                /* Falta codi */

                break;

            case 2:
                printf("Nom de fitxer en que es desara l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                fp = fopen(str1, "w");

                if(!fp){
                  printf("No es pot guardar el fitxer\n");
                  exit(0);
                }

                fwrite(&magicNumber,sizeof(int), 1, fp);



                break;

            case 3:
                //Guardamos memoria para un arbol nuevo
                tree= (rb_tree *) malloc(sizeof(rb_tree));

                init_tree(tree);
                if(!tree){
                  printf("ERROR al crear l'arbre\n");
                }
                printf("Nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                fp = open(str1, "r");

                if(!fp){
                  printf("No es pot obrir el fitxer\n");
                  exit(0);
                }

                fread(&magicNumber, sizeof(int), 1 , fp);




                break;

            case 4:
                printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                n_data = findNode(tree, str1);

                if(n_data)
                  printf("La paraula %s apareix %d vegades a l'arbre.\n", str1, n_data->num_times);
                else
                  printf("La paraula no apareix a l'arbre");

                break;

            case 5:
                //Eliminem l'abre que esta guardat a memoria y alliberem espai.
                deleteTree(tree);
                free(tree);

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}