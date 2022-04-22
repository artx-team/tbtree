#include <CUnit/Basic.h>

#include "tbtree.h"

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

static void test_tbtree_init(void)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    CU_ASSERT_TRUE(TB_EMPTY(&tree));
    CU_ASSERT_PTR_NULL(TB_FIRST(tree, &tree));
    CU_ASSERT_PTR_NULL(TB_LAST(tree, &tree));

    TB_INIT(&tree);
    CU_ASSERT_TRUE(TB_EMPTY(&tree));
    CU_ASSERT_PTR_NULL(TB_FIRST(tree, &tree));
    CU_ASSERT_PTR_NULL(TB_LAST(tree, &tree));
}

static void test_tbtree_insert(void)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    struct node *node, nodes[5];

    {
        node = &nodes[0];
        node->value = 123;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        CU_ASSERT_PTR_NULL(tmp);
        CU_ASSERT_FALSE(TB_EMPTY(&tree));
        CU_ASSERT_EQUAL(node->value, 123);

        struct node *root = TB_ROOT(&tree);
        CU_ASSERT_PTR_NOT_NULL(root);
        CU_ASSERT_PTR_EQUAL(root, node);

        struct node *head = TB_FIRST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, node);

        struct node *tail = TB_LAST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(tail, node);

        CU_ASSERT_PTR_NULL(TB_PARENT(node, entry));
        CU_ASSERT_PTR_NULL(TB_LEFT(node, entry));
        CU_ASSERT_PTR_NULL(TB_RIGHT(node, entry));
        CU_ASSERT_PTR_NULL(TB_NEXT(tree, node));
        CU_ASSERT_PTR_NULL(TB_PREV(tree, node));
        CU_ASSERT_TRUE(TB_LEAF(node, entry));
        CU_ASSERT_TRUE(TB_LLEAF(node, entry));
        CU_ASSERT_TRUE(TB_RLEAF(node, entry));
    }

    {
        node = &nodes[1];
        node->value = 123;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        CU_ASSERT_PTR_NOT_NULL(tmp);
    }

    {
        node = &nodes[2];
        node->value = 456;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        CU_ASSERT_PTR_NULL(tmp);
        CU_ASSERT_FALSE(TB_EMPTY(&tree));
        CU_ASSERT_EQUAL(node->value, 456);

        struct node *root = TB_ROOT(&tree);
        CU_ASSERT_PTR_NOT_NULL(root);
        CU_ASSERT_PTR_NOT_EQUAL(root, node);
        CU_ASSERT_EQUAL(root->value, 123);
        CU_ASSERT_FALSE(TB_LEAF(root, entry));
        CU_ASSERT_TRUE(TB_LLEAF(root, entry));
        CU_ASSERT_FALSE(TB_RLEAF(root, entry));

        struct node *head = TB_FIRST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_NOT_EQUAL(head, node);
        CU_ASSERT_EQUAL(head->value, 123);

        struct node *tail = TB_LAST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(tail, node);

        CU_ASSERT_PTR_EQUAL(TB_PARENT(node, entry), root);
        CU_ASSERT_PTR_EQUAL(TB_LEFT(node, entry), root);
        CU_ASSERT_PTR_NULL(TB_RIGHT(node, entry));
        CU_ASSERT_PTR_NULL(TB_NEXT(tree, node));
        CU_ASSERT_PTR_EQUAL(TB_PREV(tree, node), root);
        CU_ASSERT_TRUE(TB_LEAF(node, entry));
        CU_ASSERT_TRUE(TB_LLEAF(node, entry));
        CU_ASSERT_TRUE(TB_RLEAF(node, entry));
    }

    {
        node = &nodes[3];
        node->value = 0;

        struct node *tmp = TB_INSERT(tree, &tree, node);
        CU_ASSERT_PTR_NULL(tmp);
        CU_ASSERT_FALSE(TB_EMPTY(&tree));
        CU_ASSERT_EQUAL(node->value, 0);

        struct node *root = TB_ROOT(&tree);
        CU_ASSERT_PTR_NOT_NULL(root);
        CU_ASSERT_PTR_NOT_EQUAL(root, node);
        CU_ASSERT_EQUAL(root->value, 123);
        CU_ASSERT_FALSE(TB_LEAF(root, entry));
        CU_ASSERT_FALSE(TB_LLEAF(root, entry));
        CU_ASSERT_FALSE(TB_RLEAF(root, entry));

        struct node *head = TB_FIRST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, node);

        struct node *tail = TB_LAST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_NOT_EQUAL(tail, node);
        CU_ASSERT_EQUAL(tail->value, 456);

        CU_ASSERT_PTR_EQUAL(TB_PARENT(node, entry), root);
        CU_ASSERT_PTR_NULL(TB_LEFT(node, entry));
        CU_ASSERT_PTR_EQUAL(TB_RIGHT(node, entry), root);
        CU_ASSERT_PTR_EQUAL(TB_NEXT(tree, node), root);
        CU_ASSERT_PTR_NULL(TB_PREV(tree, node));
        CU_ASSERT_TRUE(TB_LEAF(node, entry));
        CU_ASSERT_TRUE(TB_LLEAF(node, entry));
        CU_ASSERT_TRUE(TB_RLEAF(node, entry));
    }

    {
        struct node *head = TB_FIRST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(head);

        head->value = 777;
        node = TB_REINSERT(tree, &tree, head);
        CU_ASSERT_PTR_NULL(node);

        struct node *tail = TB_LAST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(tail);
        CU_ASSERT_PTR_EQUAL(head, tail);

        tail->value = 0;
        node = TB_REINSERT(tree, &tree, tail);
        CU_ASSERT_PTR_NULL(node);

        head = TB_FIRST(tree, &tree);
        CU_ASSERT_PTR_NOT_NULL(head);
        CU_ASSERT_PTR_EQUAL(head, tail);
    }

    {
        struct node *root = TB_ROOT(&tree);
        CU_ASSERT_PTR_NOT_NULL(root);

        node = TB_REMOVE(tree, &tree, root);
        CU_ASSERT_PTR_EQUAL(node, root);
        CU_ASSERT_EQUAL(node->value, 123);
    }

    {
        struct node *root = TB_ROOT(&tree);
        CU_ASSERT_PTR_NOT_NULL(root);

        node = TB_REMOVE(tree, &tree, root);
        CU_ASSERT_PTR_EQUAL(node, root);
        CU_ASSERT_EQUAL(node->value, 456);
    }

    {
        struct node *root = TB_ROOT(&tree);
        CU_ASSERT_PTR_NOT_NULL(root);

        node = TB_REMOVE(tree, &tree, root);
        CU_ASSERT_PTR_EQUAL(node, root);
        CU_ASSERT_EQUAL(node->value, 0);
    }

    TB_INIT(&tree);
    CU_ASSERT_TRUE(TB_EMPTY(&tree));
    CU_ASSERT_PTR_NULL(TB_FIRST(tree, &tree));
    CU_ASSERT_PTR_NULL(TB_LAST(tree, &tree));
}

static void test_tbtree_remove_last(void)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    struct node *node, nodes[5];

    int values[] = { 1, 2, 3, 4, 5 };
    for (size_t i = 0; i < 5; ++i) {
        node = &nodes[i];
        node->value = values[i];

        struct node *tmp = TB_INSERT(tree, &tree, node);
        CU_ASSERT_PTR_NULL(tmp);

        size_t j = 0;
        TB_FOREACH(tmp, tree, &tree) {
            CU_ASSERT_PTR_NOT_NULL(tmp);
            CU_ASSERT_EQUAL(tmp->value, values[j++]);
        }
    }

    size_t i = 0;
    while ((node = TB_LAST(tree, &tree))) {
        struct node *tmp = TB_REMOVE(tree, &tree, node);
        CU_ASSERT_PTR_EQUAL(node, tmp);

        size_t j = 0;
        TB_FOREACH(tmp, tree, &tree) {
            CU_ASSERT_PTR_NOT_NULL(tmp);
            CU_ASSERT_EQUAL(tmp->value, values[j++]);
        }

        ++i;
    }

    CU_ASSERT_EQUAL(i, 5);

    TB_INIT(&tree);
    CU_ASSERT_TRUE(TB_EMPTY(&tree));
    CU_ASSERT_PTR_NULL(TB_FIRST(tree, &tree));
    CU_ASSERT_PTR_NULL(TB_LAST(tree, &tree));
}

static void test_tbtree_remove_first(void)
{
    struct tree tree = TB_HEAD_INITIALIZER(tree);
    struct node *node, nodes[5];

    int values[] = { 5, 4, 3, 2, 1 };
    for (size_t i = 0; i < 5; ++i) {
        node = &nodes[i];
        node->value = values[i];

        struct node *tmp = TB_INSERT(tree, &tree, node);
        CU_ASSERT_PTR_NULL(tmp);

        size_t j = 0;
        TB_FOREACH_REVERSE(tmp, tree, &tree) {
            CU_ASSERT_PTR_NOT_NULL(tmp);
            CU_ASSERT_EQUAL(tmp->value, values[j++]);
        }
    }

    size_t i = 0;
    while ((node = TB_FIRST(tree, &tree))) {
        struct node *tmp = TB_REMOVE(tree, &tree, node);
        CU_ASSERT_PTR_EQUAL(node, tmp);

        size_t j = 0;
        TB_FOREACH_REVERSE(tmp, tree, &tree) {
            CU_ASSERT_PTR_NOT_NULL(tmp);
            CU_ASSERT_EQUAL(tmp->value, values[j++]);
        }

        ++i;
    }

    CU_ASSERT_EQUAL(i, 5);

    TB_INIT(&tree);
    CU_ASSERT_TRUE(TB_EMPTY(&tree));
    CU_ASSERT_PTR_NULL(TB_FIRST(tree, &tree));
    CU_ASSERT_PTR_NULL(TB_LAST(tree, &tree));
}

int main(void)
{
    if (CU_initialize_registry() != CUE_SUCCESS)
        return (int)CU_get_error();

    CU_pSuite pSuite = CU_add_suite("tbtree", NULL, NULL);
    if (!pSuite)
        goto error;

    if (!CU_ADD_TEST(pSuite, test_tbtree_init) ||
        !CU_ADD_TEST(pSuite, test_tbtree_insert) ||
        !CU_ADD_TEST(pSuite, test_tbtree_remove_last) ||
        !CU_ADD_TEST(pSuite, test_tbtree_remove_first)) {
        goto error;
    }

    CU_basic_set_mode(CU_BRM_VERBOSE);
    CU_basic_run_tests();
    unsigned int num = CU_get_number_of_tests_failed();
    CU_cleanup_registry();
    if (CU_get_error() == CUE_SUCCESS) {
        return (int)num;
    } else {
        printf("CUnit Error: %s\n", CU_get_error_msg());
        return (int)CU_get_error();
    }

error:
    CU_cleanup_registry();
    return (int)CU_get_error();
}
