/**
 *
 * EASY TREE CREATION
 *
 */




rb_tree * createTree(char *pathdic,char *pathfile);//creates tree
void *thread_fn(void *par);
void indexDict(rb_tree *tree,char **dic,int size);
void indexFile(rb_tree *tree,char **file,int size);
char * createPath(char *start,char *subpath);//creation path
char **getFiles(FILE *fp_db,int *size);
void process_file1(rb_tree *tree, char *fname);
void localToGlobal(rb_tree *global,rb_tree *local);
void copyRecursive(node *global,node *local);
void index_words_line(rb_tree *tree, char *line);
void lock();
void unlock();
