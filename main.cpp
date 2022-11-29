#include <stdio.h>
#include "tree\tree.h"
#include "tree\tree_dump.h"
#include "tree\tree_text.h"

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
        $        $
        tree.root = get_g(text.buf, &char_count, &tree);
        $
        tree_graph(&tree);

$
        // fclose(output);
        // tree_dtor(&tree);
        return 0;
}
