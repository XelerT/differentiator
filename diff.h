#ifndef DIFF_H
#define DIFF_H

#include "tree\tree.h"

node_t* diff_tree (const node_t *origin_node, tree_t *tree);
int edit_temp (node_t *temp_node, int node_type, int node_data);          //returns 0 if smth wrong
node_t* create_brunch (node_t *harvest_node, node_t *l_node, node_t *r_node);
node_t* copy_brunch(node_t *node);
node_t* contain_var (node_t *node);
node_t* simplify_brunch (node_t *node, int *changed);
node_t* collapse_consts (node_t *node, int *changed);
void simplify_tree (tree_t *tree, FILE *output);

#endif /*DIFF_H*/
