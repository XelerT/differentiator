#ifndef TREE_TEX
#define TREE_TEX

#include "tree\text_tree.h"

int start_tex (tree_t *tree, const char *tex_file_name, const char *intro_file_name, FILE *output);
int convert_brunch (node_t *node, FILE *output);
void print_bkt (node_t *node, FILE *output, const char *symb);
int convert_tree (tree_t *tree, FILE *output);
int end_tex (FILE *output);

#endif /*TREE_TEX*/
