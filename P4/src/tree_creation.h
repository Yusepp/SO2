/**
 *
 * EASY TREE CREATION
 *
 */




rb_tree * createTree(char *pathdic,char *pathfile);//creates tree
void indexDict(rb_tree *tree,char **dic,int size);
void process_list(rb_tree *tree,char *mapped_names,int process);
void indexFile(rb_tree *tree,char **file,int size);
char * createPath(char *start,char *subpath);//creation path
void process_file1(rb_tree *tree, char *fname);
void index_words_line(rb_tree *tree, char *line);
