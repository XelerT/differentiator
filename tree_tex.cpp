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
        if (stat(intro_file_name, &file) < 0) {
                fprintf(stderr, "Error with stat for %s.", intro_file_name);
                return FILE_ERR;
        }

        size_t n_chars = 0;
        char *buf = (char*) calloc(file.st_size + 1, sizeof(char));
        if (!buf) {
                fprintf(stderr, "Calloc returned NULL. %d\n", __LINE__);
                return NULL_CALLOC;
        }
        n_chars = fread(buf, sizeof(char), file.st_size, intro);

        fwrite(buf, sizeof(char), n_chars, output);
        fprintf(output, "\n\\section{Introduction}\n");
        fprintf(output, "$ (");
        tex_brunch(tree->root, output);
        fprintf(output, ")\'");
        fprintf(output, "$");

        fclose(intro);
        return 0;
}

int tex_tree (tree_t *tree, FILE *output)
{
        assert(tree);
        assert(output);

        fprintf(output, "$");
        print(" =\n");
        tex_brunch(tree->root, output);
        fprintf(output, "$");

        return 0;
}

int end_tex (FILE *output)
{
        assert(output);

        fprintf(output, "\n\\end{document}");

        fclose(output);
        return 0;
}

int tex_brunch (node_t *node, FILE *output)
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
                tex_brunch(L, output);
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
                tex_brunch(R, output);
                print_bkt(node, output, ")");
                print_bkt(node, output, "}");
        }
        if (node->type == NUMBER) {
                print_bkt(node, output, "(");
                print("%llg", node->data.dbl);
                print_bkt(node, output, ")");
        } else if (node->type == VARIABLE || node->type == CONST) {
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

void tex_pdf (const char *tex_file_name)
{
        assert(tex_file_name);

        system("pdflatex");
        system("del tree.aux tree.log tree.out tree.toc");
        // system(tex_file_name);
}

void show_consts (FILE *output, const_t *consts)
{
        assert(output);
        assert(consts);

        fprintf(output, "\\section{Constants}\n");
        for (int i = 0; i < MAX_N_CONSTS && consts[i].symb != '\0'; i++) {
                printf("TEX CONSTS\n");
                fprintf(output, "\\begin{equation}\n");
                fprintf(output, "%c = ", consts[i].symb);
                tex_brunch(consts[i].node, output);
                fprintf(output, "\\end{equation}\n");
        }
}
