#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>
#include <sys/mman.h>

#define MAXCHAR 100

/**
 *
 *   map filenames of database to a anonymous mmap
 *
 */

char *dbfnames_to_mmap(FILE *fp_db)
{
  int i, size_mmap, num_files, *p_num_files;
  char line[MAXCHAR];
  char *mmap_dbfnames, *pointer;

  /* Read the number of files the database contains */
  fgets(line, MAXCHAR, fp_db);
  num_files = atoi(line);
  if (num_files <= 0) {
    printf("Number of files is %d\n", num_files);
    exit(1);
  }

  /* Create anonymous mmap */
  size_mmap = sizeof(int) + MAXCHAR * num_files; 
  mmap_dbfnames = mmap(0, size_mmap, PROT_WRITE | PROT_READ, 
      MAP_SHARED | MAP_ANONYMOUS, -1, 0);

  if (mmap_dbfnames == MAP_FAILED) {
    printf("Map failed. Errno = %d\n", errno);
    exit(1);
  }

  /* Store data in anonymous map. We begin by storing the number of 
   * files. This will be useful to munmap afterwards. */ 

  pointer = mmap_dbfnames;

  p_num_files = (int *) mmap_dbfnames;
  *p_num_files = num_files;

  pointer += sizeof(int);

  /* Read database file names */
  for(i = 0; i < num_files; i++) {
    /* Read file name */
    fgets(line, MAXCHAR, fp_db);
    line[strlen(line)-1] = 0;

    /* Copy name to mmap */
    strcpy(pointer, line);

    /* Point to new position */
    pointer += MAXCHAR;
  }

  return mmap_dbfnames;
}

/**
 *
 *   munmap database filenames from a mmap
 *
 */

void dbfnames_munmmap(char *mmap)
{
  int num_files, size_mmap;

  /* We first get the number of filenames of the mmap.
   * It is stored in the first 4 bytes of the mapping */

  num_files = * ((int *) mmap);

  size_mmap = sizeof(int) + MAXCHAR * num_files;

  munmap(mmap, size_mmap);
}

/**
 *
 *   Return filename 'i' of database. The first
 *   filename is 0.
 *
 */

char *get_dbfname_from_mmap(char *mmap, int i)
{
  int num_files;
  char *pointer;

  /* We first get the number of filenames of the mmap.
   * It is stored in the first 4 bytes of the mapping */

  num_files = * ((int *) mmap);

  /* Check if 'i' is correct */

  if (i < 0) {
    printf("ERROR in get_fname. Value of i = %d\n", i);
    exit(1);
  }

  if (i >= num_files) 
    return NULL;

  pointer = mmap + sizeof(int) + i * MAXCHAR;

  return pointer;
}
