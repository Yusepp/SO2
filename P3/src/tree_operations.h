/**
 *  
 * EASY TREE CREATION
 * 
 */

#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"
#include "read_tools.h"

#define MAXCHAR 100
#define DATABASE "./base_dades/"
#define DICTIONARY "./diccionari/"

rb_tree * createTree(char *pathdic,char *pathfile);//creates tree
char * createPath(char *start,char *subpath);//creation path