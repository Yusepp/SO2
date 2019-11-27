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
    int opcio,magicNumber;
    FILE *fp;
    rb_tree *tree;
    node_data *n_data;
    char* dic_path;
    magicNumber = MAGIC_NUMBER;
 

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

                //Escribim el magicNumber a l'inici del fitxer
                fwrite(&magicNumber, sizeof(int), 1, fp);

                //Escriu el numero de nodes a l'arbre

                fwrite(&(tree->size), sizeof(int),1,fp);

                //Escriu l'arbre a memoria
                writeTreeInicial(tree, fp);
                fclose(fp);

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

                fp = fopen(str1, "rb");

                if(!fp){
                  printf("No es pot obrir el fitxer\n");
                  exit(0);
                }
                int tmp;
                fread(&tmp,sizeof(int),1,fp);//read magicnumber
                node_data *n_data = malloc(sizeof(node_data));
                printf("%d\n",tmp);
                if(tmp == magicNumber){
                  fread(&tmp, sizeof(int), 1 , fp);//read size of tree
                  /* Initialize the tree */
                  printf("%d\n",tmp);
                  tree->size = tmp;
                  for(int i = 0; i < tmp;i++){
                    int size;
                    fread(&size,sizeof(int),1,fp);
                    printf("%d\n",size);
                    char word[size];
                    fread(&word,sizeof(char),size,fp);
                    word[strlen(word)] = 0;
                    int numkeys;
                    fread(&numkeys,sizeof(int),1,fp);

                    
                    n_data->key = word;
                    n_data->num_times = numkeys;
                    printf("%s\n",n_data->key);
                    if(find_node(tree,word) == NULL){
                      insert_node(tree,n_data);
                    }
                    

                  }
                }


                fclose(fp);






                break;

            /*
            *Quedaria que si pulsa enter aparezca la palabra que mas salga
            */
            case 4:
                printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAXCHAR, stdin);
                if(strlen(str1) == 1){
                  node_data *n_data = topWord(tree);
                  printf("La paraula %s es la que mes apareix: %d vegades a l'arbre.\n", n_data->key, n_data->num_times);
                }
                else{
                  str1[strlen(str1)-1]=0;
                  n_data = find_node(tree, str1);
                  if(n_data)
                    printf("La paraula %s apareix %d vegades a l'arbre.\n", str1, n_data->num_times);
                  else
                    printf("La paraula no apareix a l'arbre");
                }


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

