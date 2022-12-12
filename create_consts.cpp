#include <stdio.h>
#include <assert.h>
#include <string.h>
#include "create_consts.h"

int same_brunch (node_t *node1, node_t *node2)
{
        assert(node1);
        assert(node2);

        if (node1->type == node2->type) {
                switch (node1->type) {
                case NUMBER:
                        if (node1->data.dbl != node2->data.dbl)
                                return 0;
                        break;
                case VARIABLE:
                        if (node1->data.var != node2->data.var)
                                return 0;
                        break;
                case OPERATOR:
                        if (node1->data.op != node2->data.op)
                                return 0;
                        break;
                case FUNC:
                        if (strcmp(node1->func, node2->func))
                                return 0;
                        break;
                }
        } else {
                return 0;
        }

        int is_same = 1;

        if (node1->left && node2->left)
                is_same = same_brunch(node1->left, node2->left);
        if (!is_same)
                return 0;
        if (node1->right && node2->right)
                is_same = same_brunch(node1->right, node2->right);

        if (node1->type == FUNC && node2->type == FUNC) {
                if (node1->right->type == NUMBER && node2->right->type == NUMBER)
                        return !(node1->data.dbl - node2->data.dbl);
                else if (node1->right->type == VARIABLE && node2->right->type == VARIABLE)
                        return !(node1->data.var - node2->data.var);
        }

        return is_same;
}

node_t* add_consts (tree_t *tree, const_t *const_nodes)
{
        assert(tree);
        assert(const_nodes);

        if (tree->root->left)
                create_const_brunch(tree->root->left, const_nodes);
        if (tree->root->right)
                create_const_brunch(tree->root->right, const_nodes);
}

int create_const_brunch (node_t *node, const_t *consts)
{
        assert(node);

        int count = 1;
        node_t temp_node = {};

        if (node->left)
                count += create_const_brunch(node->left, consts);
        if (count > MAX_BRUNCH_SIZE) {
                if (create_const(node->left, consts, &temp_node))
                        return MANY_CONSTS;
                node->left = tree_insert(&temp_node);
        }

        count = 1;
        if (node->right)
                count += create_const_brunch(node->right, consts);
        if (count > MAX_BRUNCH_SIZE) {
                if (create_const(node->right, consts, &temp_node))
                        return MANY_CONSTS;
                node->right = tree_insert(&temp_node);
        }

        return count;
}

int create_const (node_t *node, const_t *consts, node_t *temp_node)
{
        assert(node);
        assert(consts);

        int i = 0;
        while (i < MAX_N_CONSTS) {
                if (consts[i].symb == '\0')
                        break;
                i++;
        }
        if (i <= MAX_N_CONSTS) {
                consts[i].node = node;
                consts[i].symb = 'A' + i;
                edit_temp(temp_node, CONST, consts[i].symb);
        } else {
                fprintf(stderr, "TOO MANY CONSTS\n");
                return MANY_CONSTS;
        }

        return 0;
}
