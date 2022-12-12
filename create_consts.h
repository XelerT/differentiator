#ifndef CREATE_CONSTS_H
#define CREATE_CONSTS_H

#include "tree\tree.h"
#include "diff.h"

enum const_errors {
        MANY_CONSTS = -1
};

const int MAX_N_CONSTS = 25;
const int MAX_BRUNCH_SIZE = 3;

struct const_t {
        node_t *node = nullptr;
        char symb = '\0';
};

int same_brunch (node_t *node1, node_t *node2);
node_t* add_consts (tree_t *tree, const_t *const_nodes);
int create_const_brunch (node_t *node, const_t *consts);
int create_const (node_t *node, const_t *consts, node_t *temp_node);


#endif /* CREATE_CONSTS_H */
