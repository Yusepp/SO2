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


void writeTreeData(node_data *n_data, FILE *fp);
void writeTreeInordre(node *x, FILE *fp);
void writeTreeInicial(rb_tree *tree, FILE *fp);
int countWordsTree(node *x);


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
    int opcio, magicNumber,numVegades;
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

                tree = createTree(str1,str2);

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

                int *a = malloc(sizeof(int));
                *a = 5;
                fwrite(a,sizeof(int), 1, fp);

                fclose(fp);



                //writeTreeInicial(tree, fp);


                break;

            case 3:
                //Guardamos memoria para un arbol nuevo
                tree= (rb_tree *) malloc(sizeof(rb_tree));
                int *tmp = malloc(sizeof(int));
                init_tree(tree);
                if(!tree){
                  printf("ERROR al crear l'arbre\n");
                }

                printf("Nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                fp = fopen(str1, "r");

                if(!fp){
                  printf("No es pot obrir el fitxer\n");
                  exit(0);
                }

                fread(tmp, sizeof(int), 1 , fp);
                printf("%d \n",*tmp);


                fclose(fp);






                break;

            /*
            *Quedaria que si pulsa enter aparezca la palabra que mas salga
            */
            case 4:
                printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                n_data = find_node(tree, str1);

                if(n_data)
                  printf("La paraula %s apareix %d vegades a l'arbre.\n", str1, n_data->num_times);
                else
                  printf("La paraula no apareix a l'arbre");

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

/*
*Metode per comptar el nombre de nodes que hi ha a l'arbre.
*
*/
int countWordsTree(node *x){
	int count = 1; // El seu mateix s'ha de comptar

	if(x == NIL){
		return 0;
	}
	else{
		count += countWordsTree(x->left);
		count += countWordsTree(x->right);
		return count;
	}

}
