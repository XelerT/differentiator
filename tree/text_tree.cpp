#include <stdio.h>
#include <sys\stat.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include <assert.h>

#include "tree_text.h"
#define $ fprintf(stderr, "I'm here. File %s Line %d\n", __FILE__, __LINE__);

int get_text (FILE *input, text_t *text)
{
        if (!input) {
                fprintf(stderr, "File pointer is null. File: %s | Line: %d\n", __FILE__, __LINE__);
                return NULL_FILE_PTR;
        }
        if (!text) {
                fprintf(stderr, "Text pointer is null. File: %s | Line: %d\n", __FILE__, __LINE__);
                return NULL_TEXT_PTR;
        }

        struct stat file = {};
        if (stat("diff.txt", &file) < 0)
                return FILE_ERR;

        size_t n_chars = 0;
        char *buf = (char*) calloc(file.st_size + 1, sizeof(char));
        if (!buf) {
                fprintf(stderr, "Calloc returned NULL. %d\n", __LINE__);
                return NULL_CALLOC;
        }

        n_chars = fread(buf, sizeof(char), file.st_size, input);
        text->n_chars = n_chars;

        text->buf = buf;
        text->n_lines = file.st_size - n_chars;
        if (!n_chars) {
                fprintf(stderr, "Input file is empty.\n");
                return EMPTY_FILE;
        }
        return 0;
}

void replace_n (text_t *text)
{
        if (!text) {
                fprintf(stderr, "Text pointer is null. File: %s | Line: %d\n", __FILE__, __LINE__);
                return;
        }

        char *buf = text->buf;

        for (size_t i = 0; i < text->n_chars; i++)
                if (text->buf[i] == '\n')
                        buf[i] = '\0';
}

void divide_text (text_t *text)
{
        if (!text) {
                fprintf(stderr, "Text pointer is null. File: %s | Line: %d\n", __FILE__, __LINE__);
                return;
        }

        char *buf = text->buf;
        buf[text->n_chars + 1] = '\0';

        line_t *lines = (line_t*) calloc(text->n_lines + 1, sizeof(line_t));
        if (!lines) {
                fprintf(stderr, "Calloc returned NULL. %d\n", __LINE__);
                return;
        }
        text->lines = lines;
        for (size_t i = 0; i < text->n_lines; i++) {
                lines[i].ptr = buf;
                while (*buf != '\n' && *buf != '\0')
                        buf++;
                buf++;
                lines[i].length = buf - lines[i].ptr;
        }
}

node_t* get_g (const char* str, size_t *char_count, tree_t *tree)
{
        assert(str);
        assert(char_count);
$
        node_t *node = get_e(str, char_count, tree);

        assert(*(str + *char_count) == '\0');
        return node;
}

node_t* get_e (const char* str, size_t *char_count, tree_t *tree)
{
        assert(str);
        assert(char_count);
$
        node_t *l_node = get_t(str, char_count, tree);
        node_t *node = nullptr;

        while(*(str + *char_count) == '+' || *(str + *char_count) == '-') {
                node_t temp_node = {};
                char op = *(str + *char_count);
                ++*char_count;
                node_t *r_node = get_e(str, char_count, tree);

                temp_node.type = OPERATOR;
                temp_node.data.op = op;
                node = tree_insert(tree, &temp_node);
                $
                node->left  = l_node;
                node->right = r_node;
        }

        if (node)
                return node;
        return l_node;
}

node_t* get_t (const char* str, size_t *char_count, tree_t *tree)
{
        assert(str);
        assert(char_count);
$
        node_t *l_node = get_p(str, char_count, tree);
        node_t *node = nullptr;

        while(*(str + *char_count) == '*' || *(str + *char_count) == '/') {
                node_t temp_node = {};
                char op = *(str + *char_count);
                ++*char_count;
                node_t *r_node = get_t(str, char_count, tree);

                temp_node.type = OPERATOR;
                temp_node.data.op = op;
                node = tree_insert(tree, &temp_node);
                $
                node->left  = l_node;
                printf("=-=-node data: %x %c %llg\n", node, node->left->data, node->left->data);
                node->right = r_node;
                printf("=-=-node data: %x %c %llg\n", node, node->right->data, node->right->data);
        }

        if (node)
                return node;
        return l_node;
}

node_t* get_p (const char* str, size_t *char_count, tree_t *tree)
{
        assert(str);
        assert(char_count);

        node_t* node = nullptr;
        if (*(str + *char_count) == '(') {
                ++*char_count;
                node = get_e(str, char_count, tree);
                assert(*(str + *char_count) == ')');
                ++*char_count;
        } else {
                node = get_n(str, char_count, tree);
        }

        return node;
}

node_t* get_n (const char* str, size_t *char_count, tree_t *tree)
{
        assert(str);
        assert(char_count);

        node_t node = {};
        double val = 0;
        size_t prev_char_count = *char_count;
        while ('0' <= *(str + *char_count) && *(str + *char_count) <= '9') {
                val = (double) val * 10 + *(str + *char_count) - '0';
                ++*char_count;
        }

        if ('a' <= *(str + *char_count) && *(str + *char_count) <= 'z') {
        $
                node.data.var = *(str + *char_count);
                node.type = VARIABLE;
                ++*char_count;
        } else if (val) {
        $
                node.data.dbl = val;
                printf("%s --------%llg %llg\n", __PRETTY_FUNCTION__, val, node.data);
                node.type = NUMBER;
        }

        assert(*char_count != prev_char_count);
        return tree_insert(tree, &node);
}
