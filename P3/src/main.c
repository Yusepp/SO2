
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <ctype.h>  

#include "red-black-tree.h"
#include "read_tools.h"

#define MAXCHAR 100


/**
 *
 *  Main function. This function is an example that shows
 *  how the binary tree works.
 *
 */


char * createPath(char *start,char *subpath);
void createTree(char *path,char **dic,int dic_size,int list_size,int count,char* mode);


int main(int argc, char **argv){

  FILE *s;
  s = fopen("h.bin","r");
  int t;
  fread(&t,sizeof(int),1,s);
  printf("%d\n",t);
  fread(&t,sizeof(int),1,s);
  printf("%d\n",t);
  fclose(s);
  return 0;
}


