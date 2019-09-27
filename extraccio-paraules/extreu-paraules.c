#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>  

#define MAXCHAR 100

/**
 * 
 * Given a line, extract the words it contains and print them to screen
 * 
 */ 

void process_line(char *line)
{
    int i, j, is_word, len_line;
    char paraula[MAXCHAR];

    i = 0;

    len_line = strlen(line);

    /* Search for the beginning of a candidate word */

    while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    /* This is the main loop that extracts all the words */

    while (i < len_line)
    {
        j = 0;
        is_word = 1;

        /* Extract the candidate word including digits if they are present */

        do {

            if (isalpha(line[i]))
                paraula[j] = line[i];
            else 
                is_word = 0;

            j++; i++;

            /* Check if we arrive to an end of word: space or punctuation character */

        } while ((i < len_line) && (!isspace(line[i])) && (!ispunct(line[i])));

        /* If word insert in list */

        if (is_word) {

            /* Put a '\0' (end-of-word) at the end of the string*/
            paraula[j] = 0;

            /* Print found word */
            printf("%s\n", paraula);
        }

        /* Search for the beginning of a candidate word */

        while ((i < len_line) && (isspace(line[i]) || (ispunct(line[i])))) i++; 

    } /* while (i < len_line) */
}

/**
 * 
 * Main file that accepts a filename as argument.  The function reads each of
 * the line and uses process_line to extract the words it contains.
 * 
 */


int main(int argc, char **argv)
{
    FILE *fp;
    char line[MAXCHAR];
    
    if (argc != 2) {
        printf("%s <file>\n", argv[0]);
        exit(1);
    }
    
    fp = fopen(argv[1], "r");
    if (!fp) {
        printf("Could not open file\n");
        exit(1);
    }
    
    while (fgets(line, MAXCHAR, fp))
        process_line(line);
    
    fclose(fp);

    return 0;
}
