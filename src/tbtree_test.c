#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#include "tbtree.h"

#define TEST(func) static void func(const char *__unit)

// GCOVR_EXCL_START
static void assert_expr(const char *unit, bool result,
                        const char *const expression,
                        const char *const file, const int line)
{
    if (result)
        return;

    fprintf(stderr, "%s:%d: unit test '%s' assertion failed: (%s)\n",
            file, line, unit, expression);
    fflush(stderr);
    abort();
}
// GCOVR_EXCL_STOP

#define assert_true(x) assert_expr(__unit, !!(x), #x, __FILE__, __LINE__)
#define assert_false(x) assert_true(!(x))

#define assert_null(x) assert_true((x) == NULL)
#define assert_not_null(x) assert_true((x) != NULL)

#define assert_equal(a, b) assert_true((a) == (b))
#define assert_not_equal(a, b) assert_true((a) != (b))

#define assert_str_equal(a, b) assert_true(strcmp(a, b) == 0)
#define assert_str_not_equal(a, b) assert_true(strcmp(a, b) != 0)

#define assert_strn_equal(a, b, n) assert_true(strncmp(a, b, n) == 0)
#define assert_strn_not_equal(a, b, n) assert_true(strncmp(a, b, n) != 0)

#define assert_mem_equal(a, b, n) assert_true(memcmp(a, b, n) == 0)
#define assert_mem_not_equal(a, b, n) assert_true(memcmp(a, b, n) != 0)

struct node {
    TB_ENTRY(node) entry;
    int value;
};

static inline int node_cmp(const struct node *a, const struct node *b)
{
    return (a->value > b->value) - (a->value < b->value);
}

TB_HEAD(tree, node);
TB_GENERATE_STATIC(tree, node, entry, node_cmp)

TEST(test_tbtree_init)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    assert_true(TB_EMPTY(&tree));
    assert_null(TB_FIRST(tree, &tree));
    assert_null(TB_LAST(tree, &tree));

    TB_INIT(&tree);
    assert_true(TB_EMPTY(&tree));
    assert_null(TB_FIRST(tree, &tree));
    assert_null(TB_LAST(tree, &tree));
}

TEST(test_tbtree_insert)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    struct node *node, nodes[5];

    {
        node = &nodes[0];
        node->value = 123;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        assert_null(tmp);
        assert_false(TB_EMPTY(&tree));
        assert_equal(node->value, 123);

        struct node *root = TB_ROOT(&tree);
        assert_not_null(root);
        assert_equal(root, node);

        struct node *head = TB_FIRST(tree, &tree);
        assert_not_null(head);
        assert_equal(head, node);

        struct node *tail = TB_LAST(tree, &tree);
        assert_not_null(tail);
        assert_equal(tail, node);

        assert_null(TB_PARENT(node, entry));
        assert_null(TB_LEFT(node, entry));
        assert_null(TB_RIGHT(node, entry));
        assert_null(TB_NEXT(tree, node));
        assert_null(TB_PREV(tree, node));
        assert_true(TB_LEAF(node, entry));
        assert_true(TB_LLEAF(node, entry));
        assert_true(TB_RLEAF(node, entry));
    }

    {
        node = &nodes[1];
        node->value = 123;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        assert_not_null(tmp);
    }

    {
        node = &nodes[2];
        node->value = 456;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        assert_null(tmp);
        assert_false(TB_EMPTY(&tree));
        assert_equal(node->value, 456);

        struct node *root = TB_ROOT(&tree);
        assert_not_null(root);
        assert_not_equal(root, node);
        assert_equal(root->value, 123);
        assert_false(TB_LEAF(root, entry));
        assert_true(TB_LLEAF(root, entry));
        assert_false(TB_RLEAF(root, entry));

        struct node *head = TB_FIRST(tree, &tree);
        assert_not_null(head);
        assert_not_equal(head, node);
        assert_equal(head->value, 123);

        struct node *tail = TB_LAST(tree, &tree);
        assert_not_null(tail);
        assert_equal(tail, node);

        assert_equal(TB_PARENT(node, entry), root);
        assert_equal(TB_LEFT(node, entry), root);
        assert_null(TB_RIGHT(node, entry));
        assert_null(TB_NEXT(tree, node));
        assert_equal(TB_PREV(tree, node), root);
        assert_true(TB_LEAF(node, entry));
        assert_true(TB_LLEAF(node, entry));
        assert_true(TB_RLEAF(node, entry));
    }

    {
        node = &nodes[3];
        node->value = 0;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        assert_null(tmp);
        assert_false(TB_EMPTY(&tree));
        assert_equal(node->value, 0);

        struct node *root = TB_ROOT(&tree);
        assert_not_null(root);
        assert_not_equal(root, node);
        assert_equal(root->value, 123);
        assert_false(TB_LEAF(root, entry));
        assert_false(TB_LLEAF(root, entry));
        assert_false(TB_RLEAF(root, entry));

        struct node *head = TB_FIRST(tree, &tree);
        assert_not_null(head);
        assert_equal(head, node);

        struct node *tail = TB_LAST(tree, &tree);
        assert_not_null(tail);
        assert_not_equal(tail, node);
        assert_equal(tail->value, 456);

        assert_equal(TB_PARENT(node, entry), root);
        assert_null(TB_LEFT(node, entry));
        assert_equal(TB_RIGHT(node, entry), root);
        assert_equal(TB_NEXT(tree, node), root);
        assert_null(TB_PREV(tree, node));
        assert_true(TB_LEAF(node, entry));
        assert_true(TB_LLEAF(node, entry));
        assert_true(TB_RLEAF(node, entry));
    }

    {
        struct node *head = TB_FIRST(tree, &tree);
        assert_not_null(head);

        head->value = 777;
        node = TB_REINSERT(tree, &tree, head);
        assert_null(node);

        struct node *tail = TB_LAST(tree, &tree);
        assert_not_null(tail);
        assert_equal(head, tail);

        tail->value = 0;
        node = TB_REINSERT(tree, &tree, tail);
        assert_null(node);

        head = TB_FIRST(tree, &tree);
        assert_not_null(head);
        assert_equal(head, tail);
    }

    {
        struct node *root = TB_ROOT(&tree);
        assert_not_null(root);

        node = TB_REMOVE(tree, &tree, root);
        assert_equal(node, root);
        assert_equal(node->value, 123);
    }

    {
        struct node *root = TB_ROOT(&tree);
        assert_not_null(root);

        node = TB_REMOVE(tree, &tree, root);
        assert_equal(node, root);
        assert_equal(node->value, 456);
    }

    {
        struct node *root = TB_ROOT(&tree);
        assert_not_null(root);

        node = TB_REMOVE(tree, &tree, root);
        assert_equal(node, root);
        assert_equal(node->value, 0);
    }

    TB_INIT(&tree);
    assert_true(TB_EMPTY(&tree));
    assert_null(TB_FIRST(tree, &tree));
    assert_null(TB_LAST(tree, &tree));
}

TEST(test_tbtree_remove_last)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    struct node *node, nodes[5];

    int values[] = { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < 5; ++i) {
        node = &nodes[i];
        node->value = values[i];

        struct node *tmp = TB_INSERT(tree, &tree, node);
        assert_null(tmp);

        size_t j = 0;
        TB_FOREACH(tmp, tree, &tree) {
            assert_not_null(tmp);
            assert_equal(tmp->value, values[j++]);
        }
    }

    size_t i = 0;
    while ((node = TB_LAST(tree, &tree))) {
        struct node *tmp = TB_REMOVE(tree, &tree, node);
        assert_equal(node, tmp);

        size_t j = 0;
        TB_FOREACH(tmp, tree, &tree) {
            assert_not_null(tmp);
            assert_equal(tmp->value, values[j++]);
        }

        ++i;
    }

    assert_equal(i, 5);

    TB_INIT(&tree);
    assert_true(TB_EMPTY(&tree));
    assert_null(TB_FIRST(tree, &tree));
    assert_null(TB_LAST(tree, &tree));
}

TEST(test_tbtree_remove_first)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    struct node *node, nodes[5];

    int values[] = { 5, 4, 3, 2, 1 };
    for (size_t i = 0; i < 5; ++i) {
        node = &nodes[i];
        node->value = values[i];

        struct node *tmp = TB_INSERT(tree, &tree, node);
        assert_null(tmp);

        size_t j = 0;
        TB_FOREACH_REVERSE(tmp, tree, &tree) {
            assert_not_null(tmp);
            assert_equal(tmp->value, values[j++]);
        }
    }

    size_t i = 0;
    while ((node = TB_FIRST(tree, &tree))) {
        struct node *tmp = TB_REMOVE(tree, &tree, node);
        assert_equal(node, tmp);

        size_t j = 0;
        TB_FOREACH_REVERSE(tmp, tree, &tree) {
            assert_not_null(tmp);
            assert_equal(tmp->value, values[j++]);
        }

        ++i;
    }

    assert_equal(i, 5);

    TB_INIT(&tree);
    assert_true(TB_EMPTY(&tree));
    assert_null(TB_FIRST(tree, &tree));
    assert_null(TB_LAST(tree, &tree));
}

int main(void)
{
    struct {
        const char *name;
        void (*func)(const char *__unit);
    } tests[] = {
        { "tbtree_init", test_tbtree_init },
        { "tbtree_insert", test_tbtree_insert },
        { "tbtree_remove_last", test_tbtree_remove_last },
        { "tbtree_remove_first", test_tbtree_remove_first },
    };

    for (size_t i = 0, n = sizeof(tests) / sizeof(tests[0]); i < n; ++i) {
        printf(">>> Testing (%zu of %zu) %s...\n", i + 1, n, tests[i].name);
        tests[i].func(tests[i].name);
    }

    return EXIT_SUCCESS;
}
