#include <stdio.h>
#include "tree\tree.h"
#include "tree\tree_dump.h"
#include "tree\text_tree.h"
#include "diff.h"
#include "tree_tex.h"

#define $ fprintf(stderr, "I'm here. File %s Line %d\n", __FILE__, __LINE__);

int main ()
{
        tree_t tree = {};
        text_t text = {};
        tree_ctor(&tree);
        FILE *diff = fopen("diff.txt", "r");
        if (!diff) {
                fprintf(stderr, "File pointer is null. %d\n", __LINE__);
                return NULL_FILE_PTR;
        }
        int error = 0;
        if (error = get_text (diff, &text))
                return error;
        replace_n (&text);
        divide_text (&text);
        size_t char_count = 0;
        tree.root = get_g(text.buf, &char_count, &tree);
        tree_graph(&tree, "tree_graph.dot");

        FILE *output = fopen("tree.tex", "w");
        if (!output) {
                fprintf(stderr, "File %s pointer is null.\n", "tree.tex");
                return NULL_FILE_PTR;
        }

        start_tex(&tree, "tree.tex", "intro.txt", output);

        tree_t diffed_tree = {};
        tree_ctor(&diffed_tree);
        const_t const_nodes[MAX_N_CONSTS] = {};

        diffed_tree.root = diff_tree((const node_t*) tree.root, &diffed_tree);
        add_consts(&diffed_tree, const_nodes);

        tex_tree(&diffed_tree, output);
        simplify_tree(&diffed_tree, output);
$
        tree_graph(&diffed_tree, "diffed_graph.dot");
        show_consts(output, const_nodes);
        end_tex(output);
        tex_pdf("tree.tex");
        fclose(diff);
        tree_dtor(&diffed_tree);
        tree_dtor(&tree);
        return 0;
}
