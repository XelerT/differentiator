#ifndef TREE_DUMP_H
#define TREE_DUMP_H

#include "tree.h"

struct edge_t {
        int penwidth = 5;
        const char *color = "black";
};

struct digraph_t {
        int dpi = 300;
        const char *splines = "ortho";
};

int tree_dump (tree_t *tree, const char *file_name, const char *func, const int line, const char *tree_name);
int tree_error (tree_t *tree);
void tie_gv_nodes (node_t *node);
void print_gv_nodes (node_t *node);
void tree_graph (tree_t *tree);

#endif /*TREE_DUMP_H*/
