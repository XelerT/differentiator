#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <math.h>

#include "diff.h"
#include "tree_tex.h"

#include "tree\tree_dump.h"
#include "tree\text_tree.h"

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
                        case DEG:
                                edit_temp(&temp_node, OPERATOR, MUL);
                                new_node = ins_temp;
                                node_t temp_node2 = {};
                                $

                                if (contain_var(origin_node->right)) {
                                        $
                                        if (contain_var(origin_node->left)) {
                                                edit_temp(&temp_node2, OPERATOR, ADD);
                                                L = create_brunch(tree_insert(&temp_node2), ins_temp, ins_temp);
                                                L->left->left = diff(left);
                                                temp_node2.type = FUNC;
                                                strcpy(temp_node2.func, "ln");
                                                L->left->right = tree_insert(&temp_node2);
                                                L->left->right->right = ins_origin(right);

                                                edit_temp(&temp_node2, OPERATOR, DIV);
                                                L->right->left = create_brunch(ins_temp, ins_origin(left), tree_insert(&temp_node2));
                                                L->right->right = diff(right);

                                                edit_temp(&temp_node, NUMBER, 1);
                                                L->right->left->right->left = tree_insert(&temp_node);
                                                L->right->left->right->right = ins_origin(right);
                                                R = copy_brunch((node_t*) origin_node);

                                                return new_node;
                                        }
                                        $
                                        // edit_temp(&temp_node2, FUNC, "ln");
                                        temp_node2.type = FUNC;
                                        strcpy(temp_node2.func, "ln");
                                        L = create_brunch(ins_temp, copy_brunch((node_t*) origin_node), tree_insert(&temp_node2));
                                        L->right->right = ins_origin(left);
                                        R = diff(right);
                                } else if (contain_var(origin_node->left)) {
                                        edit_temp(&temp_node2, OPERATOR, DEG);
                                        L = create_brunch(ins_temp, ins_origin(right), tree_insert(&temp_node2));

                                        edit_temp(&temp_node, NUMBER, origin_node->right->data.dbl - 1);
                                        L->right->left = ins_origin(left);
                                        L->right->right = ins_temp;
                                        R = diff(left);
                                } else {
                                        edit_temp(&temp_node, NUMBER, 0);
                                        new_node = tree_insert(&temp_node);
                                }
                                printf("Left has var %x\n", contain_var(origin_node->left));

                                return new_node;
                }
        case VARIABLE:
                edit_temp(&temp_node, NUMBER, 1);
                new_node = ins_temp;
                $
                return new_node;
        case FUNC:
                node_t temp_node2 = {};
                if (!stricmp(origin_node->func, "sin")) {
                        edit_temp(&temp_node, OPERATOR, MUL);
                        strcpy(temp_node2.func, "cos");
                        temp_node2.type = FUNC;
                        temp_node2.data.dbl = -1;

                        new_node = create_brunch(ins_temp, tree_insert(&temp_node2), diff(right));
                        L->right = ins_origin(right);

                } else if (!stricmp(origin_node->func, "cos")) {
                        $
                        edit_temp(&temp_node, OPERATOR, MUL);
                        strcpy(temp_node2.func, "sin");
                        temp_node2.type = FUNC;
                        temp_node2.data.dbl = -1;

                        new_node = create_brunch(ins_temp, ins_temp, diff(right));

                        edit_temp(&temp_node, NUMBER, -1);
                        L->left = tree_insert(&temp_node);
                        L->right = tree_insert(&temp_node2);
                        L->right->left = ins_origin(right);
                } else if (!stricmp(origin_node->func, "ln")) {
                        $
                        edit_temp(&temp_node, OPERATOR, MUL);
                        edit_temp(&temp_node2, OPERATOR, DIV);

                        new_node = create_brunch(ins_temp, tree_insert(&temp_node2), diff(right));
                        edit_temp(&temp_node, NUMBER, 1);
                        L->left  = ins_temp;
                        L->right = ins_origin(right);
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

node_t* contain_var (node_t *node)
{
        assert(node);

        node_t *var_node = nullptr;

        if (node->type == VARIABLE)
                return node;
        if (node->left) {
                var_node = contain_var(node->left);
                if (var_node)
                        return var_node;
        }
        if (node->right) {
                var_node = contain_var(node->right);
        }

        return var_node;
}

node_t* collapse_consts (node_t *node, int *changed)
{
        assert(node);

        if (node->left) {
        $
                // getchar();
                // printf("%x-------------------type %d-----------------------%llg\n", node->left, node->type, node->data.dbl);
                node->left  = collapse_consts(node->left, changed);
        $
        }
        $
        if (node->right) {
        $
                node->right = collapse_consts(node->right, changed);
        $
        }
$
        if (node->left && node->right)
                if (node->left->type == NUMBER && node->right->type == NUMBER) {
                        if (node->type == OPERATOR) {
                        $
                                elem_t data;
                                switch (node->data.op) {
                                case ADD:
                                        data.dbl = node->left->data.dbl + node->right->data.dbl;
                                        break;
                                case SUB:
                                        data.dbl = node->left->data.dbl - node->right->data.dbl;
                                        break;
                                case MUL:
                                        data.dbl = node->left->data.dbl * node->right->data.dbl;
                                        break;
                                case DIV:
                                        data.dbl = node->left->data.dbl / node->right->data.dbl;
                                        break;
                                case DEG:
                                        data.dbl = pow(node->left->data.dbl, node->right->data.dbl);
                                        break;
                                }
                                node->type = NUMBER;
                                node->data.dbl = data.dbl;
                                *changed = 1;
$
                                free(node->left);
                                free(node->right);
                                node->left  = nullptr;
                                node->right = nullptr;
                        }
                }
$
        return node;
}

node_t* simplify_brunch (node_t *node, int *changed)
{
        assert(node);
        $
        node_t *temp_node = node;

        if (node->left) {
        $
                node->left  = simplify_brunch(node->left, changed);
        }
        if (node->right) {
        $
                node->right = simplify_brunch(node->right, changed);
        }
        $
        if (node->type == OPERATOR) {
        // printf("func %s\n", node->func);
                switch (node->data.op) {
                case ADD:
                        if (node->left->type == NUMBER) {
                                if (node->left->data.dbl == 0) {
                                        temp_node = node->right;
                                        free(node->left);
                                        node->left = nullptr;
                                        free(node);
                                        node = nullptr;
                                }
                        } else if (node->right->type == NUMBER) {
                                if (node->right->data.dbl == 0) {
                                        temp_node = node->left;
                                        free(node->right);
                                        node->right = nullptr;
                                        free(node);
                                        node = nullptr;
                                }
                        }
                        break;
                case MUL:
                        if (node->left->type == NUMBER || node->right->type == NUMBER) {
                                if ((node->left->data.dbl == 0 || node->right->data.dbl == 0)
                                     && node->left->type != FUNC && node->right->type != FUNC) {
                                        $
                                        node->type = NUMBER;
                                        node->data.dbl = 0;
                                        temp_node = node;
                                        free(node->right);
                                        node->right = nullptr;
                                        free(node->left);
                                        node->left = nullptr;

                                        return temp_node;
                                }
                        }
                        if (node->left->type == NUMBER) {
                                if (node->left->data.dbl == 1) {
                                        temp_node = node->right;
                                        free(node->left);
                                        node->left = nullptr;
                                        free(node);
                                        node = nullptr;
                                }
                        } else if (node->right->type == NUMBER) {
                                if (node->right->data.dbl == 1) {
                                        temp_node = node->left;
                                        free(node->right);
                                        node->right = nullptr;
                                        free(node);
                                        node = nullptr;
                                }
                        }
                        break;
                case DEG:
                        if (node->left->type == NUMBER || node->right->type == NUMBER) {
                                if (node->left->data.dbl == 1 || node->right->data.dbl == 1) {
                                        temp_node = node->left;
                                        free(node->right);
                                        node->right = nullptr;
                                        free(node);
                                        node = nullptr;
                                }
                        }
                        break;
                }
        }
        $
        if (!node)
                *changed = 1;

        return temp_node;
}

void simplify_tree (tree_t *tree, FILE *output)
{
        assert(tree);

        int changed = 0;

        do {
                changed = 0;

                tree->root = simplify_brunch(tree->root, &changed);
                $
                collapse_consts(tree->root, &changed);
                $
                tex_tree(tree, output);
        } while (changed);
}
