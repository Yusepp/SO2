#pragma once

char *dbfnames_to_mmap(FILE *fp_db);
void dbfnames_munmmap(char *mmap);
char *get_dbfname_from_mmap(char *mmap, int i);



