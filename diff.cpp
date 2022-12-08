#include <stdio.h>
#include <assert.h>
#include <string.h>

#include "diff.h"

#define diff(side) diff_tree(origin_node->side, tree)
#define ins_temp tree_insert(&temp_node)
#define ins_origin(side) copy_brunch(origin_node->side)

#define L new_node->left
#define R new_node->right

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
                return tree_insert(&temp_node);
        case OPERATOR:
                switch (origin_node->data.op) {
                        case ADD:
                        case SUB:
                                $
                                new_node = create_brunch(tree_insert(origin_node), diff(left), diff(right));

                                return new_node;
                        case MUL:
                                new_node = tree_insert(origin_node);
                                new_node->data.op = ADD;

                                edit_temp(&temp_node, OPERATOR, MUL);

                                L = create_brunch(ins_temp, diff(left), ins_origin(right));
                                R = create_brunch(ins_temp, ins_origin(left), diff(right));

                                return new_node;
                        case DIV:
                                new_node = tree_insert(origin_node);

                                edit_temp(&temp_node, OPERATOR, SUB);
                                L = ins_temp;

                                edit_temp(&temp_node, OPERATOR, MUL);

                                L->left = create_brunch(ins_temp, diff(left), ins_origin(right));
                                L->right = create_brunch(ins_temp, ins_origin(left), diff(right));
                                R = create_brunch(ins_temp, ins_origin(left), ins_origin(right));        //create denomiator x*x

                                return new_node;
                }
        case VARIABLE:
                edit_temp(&temp_node, NUMBER, 1);
                new_node = ins_temp;
                $
                return new_node;
        case FUNC:
                node_t temp_node2 = {};
                $
                if (!stricmp(origin_node->func, "sin")) {
                $
                        edit_temp(&temp_node, OPERATOR, MUL);
                        strcpy(temp_node2.func, "cos");
                        temp_node2.type = FUNC;
                        $
                        new_node = create_brunch(ins_temp, tree_insert(&temp_node2), diff(right));
                        $
                        L->right = ins_origin(right);

                } else if (!stricmp(origin_node->func, "cos")) {
                        $
                } else if (!stricmp(origin_node->func, "ln")) {
                        $
                }
                $
                return new_node;
        }
}

int edit_temp (node_t *temp_node, int node_type, int node_data)          //returns 0 if smth wrong
{
        assert(temp_node);

        temp_node->type = node_type;
        switch (node_type) {
        case NUMBER:
                temp_node->data.dbl = node_data;
                return NUMBER;
        case OPERATOR:
                temp_node->data.op = node_data;
                return OPERATOR;
        case VARIABLE:
                temp_node->data.var = node_data;
                return VARIABLE;
        // case FUNC:
        //         temp_node->func
        default:
                return 0;
        }
}

node_t* create_brunch (node_t *harvest_node, node_t *l_node, node_t *r_node)
{
        assert(harvest_node);
        assert(l_node);
        assert(r_node);

        node_t *new_node  = tree_insert(harvest_node);
        new_node->left = l_node;
        new_node->right = r_node;

        return new_node;
}

node_t* copy_brunch(node_t *node)
{
        $
        assert(node);

        node_t *new_node = tree_insert(node);
        if (node->left) {
               new_node->left = copy_brunch(node->left);
        }
        if (node->right) {
                new_node->right = copy_brunch(node->right);
        }

        return new_node;
}
