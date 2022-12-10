#include <stdio.h>
#include <assert.h>
#include <string.h>
#include <sys\stat.h>
#include "tree_tex.h"

#define print(...) fprintf(output, __VA_ARGS__)
#define L node->left
#define R node->right

int start_tex (tree_t *tree, const char *tex_file_name, const char *intro_file_name, FILE *output)
{
        assert(tree);
        assert(tex_file_name);

        FILE *intro  = fopen(intro_file_name, "r");
        if (!intro) {
                fprintf(stderr, "File %s pointer is null.\n", intro_file_name);
                return NULL_FILE_PTR;
        }
        $
        struct stat file = {};
        if (stat(intro_file_name, &file) < 0)
                return FILE_ERR;

        size_t n_chars = 0;
        char *buf = (char*) calloc(file.st_size + 1, sizeof(char));
        if (!buf) {
                fprintf(stderr, "Calloc returned NULL. %d\n", __LINE__);
                return NULL_CALLOC;
        }
        n_chars = fread(buf, sizeof(char), file.st_size, intro);

        fwrite(buf, sizeof(char), n_chars, output);
        fprintf(output, "\n\\section{Introduction}\n");
        convert_brunch(tree->root, output);

        fclose(intro);
        return 0;
}

int convert_tree (tree_t *tree, FILE *output)
{
        assert(tree);
        assert(output);

        print(" =\n");
        convert_brunch(tree->root, output);

        return 0;
}

int end_tex (FILE *output)
{
        assert(output);

        fprintf(output, "\n\\end{document}");

        fclose(output);
}

int convert_brunch (node_t *node, FILE *output)
{
        assert(node);
        assert(output);

        if (node->type == OPERATOR) {
                if (node->data.op == '/')
                        print("\\frac{");
        }
        if (node->type == FUNC)
                        print("\\%s", node->func);

$
        if (L) {
                print_bkt(node, output, "(");
                convert_brunch(L, output);
                print_bkt(node, output, ")");
                if (node->data.op == '*')
                        print("\\cdot ");
                else if (node->data.op != '/')
                        print("%c", node->data.op);
        }
        if (node->type == OPERATOR) {
                if (node->data.op == '/')
                        print_bkt(node, output, "}");
        }
        if (R) {
                print_bkt(node, output, "{");
                print_bkt(node, output, "(");
                convert_brunch(R, output);
                print_bkt(node, output, ")");
                print_bkt(node, output, "}");
        }
        if (node->type == NUMBER) {
                print_bkt(node, output, "(");
                print("%llg", node->data.dbl);
                print_bkt(node, output, ")");
        } else if (node->type == VARIABLE) {
                print("%c", node->data.var);
        }
}

void print_bkt (node_t *node, FILE *output, const char *symb)
{
        assert(node);
        assert(symb);

        switch(node->type) {
        case OPERATOR:
                if (node->data.op == '*' && strcmp(symb, "{") && strcmp(symb, "}")) {
                        if (L->type == OPERATOR || R->type == OPERATOR)
                                if (L->data.op == '+' || L->data.op == '-' ||
                                    R->data.op == '+' || R->data.op == '-')
                                            print(symb);
                } else if (node->data.op == '/' || node->data.op == '^' && strcmp(symb, "(") && strcmp(symb, ")"))
                        print(symb);
                break;
        case FUNC:
                if (strcmp(symb, "{") && strcmp(symb, "}"))
                        print(symb);
                break;
        case NUMBER:
                if (node->data.dbl < 0 && strcmp(symb, "{") && strcmp(symb, "}")) {
                        print(symb);
                }
                break;
        }
}

#undef print
#undef L
#undef R
