#include <stdio.h>
#include <assert.h>

#include "diff.h"

#define next_node(new) new          = tree_insert(tree, origin_node);                   \
                       new->left  = tree_insert(tree, diff_tree(origin_node->left,  tree));             \
                       new->right = tree_insert(tree, diff_tree(origin_node->right, tree));

#define diff(side) diff_tree(origin_node->side, tree)
#define ins_temp tree_insert(tree, &temp_node)
#define ins_origin(side) tree_insert(tree, origin_node->side)

#define edit_temp(node_type,node_data)  temp_node.type = node_type;                             \
                                        switch (node_type) {                                    \
                                                case NUMBER:                                    \
                                                        temp_node.data.dbl = node_data;         \
                                                        break;                                  \
                                                case OPERATOR:                                  \
                                                        temp_node.data.op = node_data;          \
                                                        break;                                  \
                                                case VARIABLE:                                  \
                                                        temp_node.data.var = node_data;         \
                                                        break;                                  \
                                        }

node_t* diff_tree (const node_t *origin_node, tree_t *tree)
{
        assert(origin_node);
        assert(tree);

        node_t temp_node = {};
        node_t *new_node = nullptr;
        $

        switch (origin_node->type) {
        case NUMBER:
                $
                temp_node.data.dbl = 0;
                temp_node.type = NUMBER;
                return tree_insert(tree, &temp_node);
        case OPERATOR:
                switch (origin_node->data.op) {
                        case ADD:
                        case SUB:
                                $
                                new_node        = tree_insert(tree, origin_node);
                                new_node->left  = diff(left);
                                new_node->right = diff(right);
                                return new_node;

                        case MUL:
                                new_node = tree_insert(tree, origin_node);
                                new_node->data.op = ADD;

                                edit_temp(OPERATOR, MUL);

                                new_node->left = ins_temp;
                                new_node->left->left = diff(left);
                                new_node->left->right = ins_origin(right);


                                new_node->right = ins_temp;
                                new_node->right->left = ins_origin(left);
                                new_node->right->right = diff(right);

                                return new_node;
                        case DIV:
                                $
                                new_node = tree_insert(tree, origin_node);

                                edit_temp(OPERATOR, SUB);
                                new_node->left = ins_temp;

                                edit_temp(OPERATOR, MUL);

                                new_node->left->left  = ins_temp;
                                new_node->left->right = ins_temp;

                                new_node->left->left->left = diff(left);
                                new_node->left->left->right = ins_origin(right);

                                new_node->left->right->left = ins_origin(left);
                                new_node->left->right->right = diff(right);

                                new_node->right = ins_temp;                                     //create denomiator x*x
                                new_node->right->left = ins_origin(left);
                                new_node->right->right = ins_origin(right);

                                return new_node;
                }
        case VARIABLE:
                int one = 1;
                edit_temp(NUMBER, one);
                new_node = ins_temp;
                $
                return new_node;
        }
}

#undef next_node
