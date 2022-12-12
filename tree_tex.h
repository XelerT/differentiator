#ifndef TREE_TEX
#define TREE_TEX

#include "tree\text_tree.h"
#include "create_consts.h"

int start_tex (tree_t *tree, const char *tex_file_name, const char *intro_file_name, FILE *output);
int tex_brunch (node_t *node, FILE *output);
void print_bkt (node_t *node, FILE *output, const char *symb);
int tex_tree (tree_t *tree, FILE *output);
int end_tex (FILE *output);
void tex_pdf (const char *tex_file_name);
void show_consts (FILE *output, const_t *consts);

#endif /*TREE_TEX*/
