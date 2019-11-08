/**
 *
 * Practica 3 
 *
 */

#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
    char * line = NULL;
    size_t len = 0;
    size_t read;
    int magicNumber = MAGIC_NUMBER;
    unsigned char buffer[10];
    char str1[MAXCHAR], str2[MAXCHAR];
    int opcio;
    int num;
    FILE *fptr;
    FILE *ptr;    
    
    
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
                ptr = fopen(str1, "w"); // w for write, b for binary
                if(ptr == NULL){
                    printf("Error!");   
                    exit(1);             
                }
                fwrite(&magicNumber,sizeof(int), 1,ptr); // write 10 bytes from our buffer
                fclose(ptr);
                break;

            case 3:
                printf("Nom del fitxer que conte l'arbre: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                //ptr = fopen("prova.bin","rb");  // r for read, b for binary
                //fread(buffer,sizeof(buffer),1,ptr); // read 10 bytes to our buffer

                

                fp = fopen(str1, "r");
                if (fp == NULL)
                    exit(EXIT_FAILURE);

                while ((read = getline(&line, &len, fp)) != -1) {
                    printf("Retrieved line of length %zu:\n", read);
                    printf("%s", line);
                }

                fclose(fp);
                if (line)
                    free(line);
                exit(EXIT_SUCCESS);
                break;

            case 4:
                printf("Paraula a buscar o polsa enter per saber la paraula que apareix mes vegades: ");
                fgets(str1, MAXCHAR, stdin);
                str1[strlen(str1)-1]=0;

                /* Falta codi */

                break;

            case 5:

                /* Falta codi */

                break;

            default:
                printf("Opcio no valida\n");

        } /* switch */
    }
    while (opcio != 5);

    return 0;
}

