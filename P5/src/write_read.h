
void writeTree(FILE *fp,rb_tree * tree,int magicNumber);
rb_tree *readTree(FILE * fp,int magicNumber);
void writeTreeData(node_data *n_data, FILE *fp);
void writeTreeInordre(node *x, FILE *fp);
void writeTreeInicial(rb_tree *tree, FILE *fp);