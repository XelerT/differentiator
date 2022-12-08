#ifndef DIFF_H
#define DIFF_H

#include "tree\tree.h"

node_t* diff_tree (const node_t *origin_node, tree_t *tree);
int edit_temp (node_t *temp_node, int node_type, int node_data);          //returns 0 if smth wrong
node_t* create_brunch (node_t *harvest_node, node_t *l_node, node_t *r_node);

#endif /*DIFF_H*/
