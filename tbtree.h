#pragma once

/* TBTREE is a single header intrusive double threaded binary tree.
 *
 * It is inspired by `<sys/tree.h>` from `FreeBSD`.
 *
 * Reference: https://en.wikipedia.org/wiki/Threaded_binary_tree
 */

#ifndef __tbtree_has_attribute
#   ifdef __has_attribute
#       define __tbtree_has_attribute(x) __has_attribute(x)
#   else
#       define __tbtree_has_attribute(x) (0)
#   endif
#endif

#ifndef __tbtree_unused
#   if defined(__GNUC__) || __tbtree_has_attribute(__unused__)
#       define __tbtree_unused __attribute__((__unused__))
#   else
#       define __tbtree_unused
#   endif
#endif

#include <stdint.h>

#define TB_HEAD(name, type) \
    struct name { \
        struct type *tb_root; \
    }

#define TB_HEAD_INITIALIZER(head) \
    { NULL }

#define TB_INIT(head) do { \
        (head)->tb_root = NULL; \
    } while (0)

#define TB_ENTRY(type) \
    struct { \
        struct type *tb_left; \
        struct type *tb_right; \
        struct type *tb_parent; \
    }

#define TB_ROOT(head)           ((head)->tb_root)
#define TB_EMPTY(head)          (TB_ROOT(head) == NULL)

#define TB_LEFT(elm, field)     ((elm)->field.tb_left)
#define TB_RIGHT(elm, field)    ((elm)->field.tb_right)

#define TB_UP(elm, field)       ((elm)->field.tb_parent)
#define TB_BITS(elm, field)     (*(uintptr_t *)&TB_UP(elm, field))

#define TB_LBIT                 ((uintptr_t)1)
#define TB_RBIT                 ((uintptr_t)2)
#define TB_MASK                 ((uintptr_t)3)

#define TB_SET(elm, field)      (TB_BITS(elm, field) |= TB_MASK)
#define TB_LSET(elm, field)     (TB_BITS(elm, field) |= TB_LBIT)
#define TB_RSET(elm, field)     (TB_BITS(elm, field) |= TB_RBIT)

#define TB_CLEAR(elm, field)    (TB_BITS(elm, field) &= ~TB_MASK)
#define TB_LCLEAR(elm, field)   (TB_BITS(elm, field) &= ~TB_LBIT)
#define TB_RCLEAR(elm, field)   (TB_BITS(elm, field) &= ~TB_RBIT)

#define TB_LEAF(elm, field)     ((TB_BITS(elm, field) & TB_MASK) == TB_MASK)
#define TB_LLEAF(elm, field)    ((TB_BITS(elm, field) & TB_LBIT) != 0)
#define TB_RLEAF(elm, field)    ((TB_BITS(elm, field) & TB_RBIT) != 0)

#define TB_PARENT(elm, field)   ((__typeof__(TB_UP(elm, field))) \
                                 (TB_BITS(elm, field) & ~TB_MASK))

#define TB_SET_PARENT(elm, parent, field) do { \
        TB_BITS(elm, field) &= TB_MASK; \
        TB_BITS(elm, field) |= (uintptr_t)(parent); \
    } while (0)

#define TB_SWAP_CHILD(head, parent, out, in, field) do { \
        if ((parent) == NULL) { \
            TB_ROOT(head) = (in); \
        } else if (TB_LEFT(parent, field) == (out)) { \
            TB_LEFT(parent, field) = (in); \
        } else { \
            TB_RIGHT(parent, field) = (in); \
        } \
    } while (0)

#define TB_PROTOTYPE(name, type, field, cmp) \
    TB_PROTOTYPE_INTERNAL(name, type, field, cmp,)

#define TB_PROTOTYPE_STATIC(name, type, field, cmp) \
    TB_PROTOTYPE_INTERNAL(name, type, field, cmp, __tbtree_unused static)

#define TB_PROTOTYPE_INTERNAL(name, type, field, cmp, attr) \
    TB_PROTOTYPE_MIN(name, type, attr); \
    TB_PROTOTYPE_MAX(name, type, attr); \
    TB_PROTOTYPE_PREV(name, type, attr); \
    TB_PROTOTYPE_NEXT(name, type, attr); \
    TB_PROTOTYPE_FIRST(name, type, attr); \
    TB_PROTOTYPE_LAST(name, type, attr); \
    TB_PROTOTYPE_FIND(name, type, attr); \
    TB_PROTOTYPE_NFIND(name, type, attr); \
    TB_PROTOTYPE_INSERT(name, type, attr); \
    TB_PROTOTYPE_REMOVE(name, type, attr); \
    TB_PROTOTYPE_REINSERT(name, type, attr); \

#define TB_PROTOTYPE_MIN(name, type, attr) \
    attr struct type *name##_TB_MIN(struct type *)

#define TB_PROTOTYPE_MAX(name, type, attr) \
    attr struct type *name##_TB_MAX(struct type *)

#define TB_PROTOTYPE_PREV(name, type, attr) \
    attr struct type *name##_TB_PREV(struct type *)

#define TB_PROTOTYPE_NEXT(name, type, attr) \
    attr struct type *name##_TB_NEXT(struct type *)

#define TB_PROTOTYPE_FIRST(name, type, attr) \
    attr struct type *name##_TB_FIRST(struct name *)

#define TB_PROTOTYPE_LAST(name, type, attr) \
    attr struct type *name##_TB_LAST(struct name *)

#define TB_PROTOTYPE_FIND(name, type, attr) \
    attr struct type *name##_TB_FIND(struct name *, struct type *)

#define TB_PROTOTYPE_NFIND(name, type, attr) \
    attr struct type *name##_TB_NFIND(struct name *, struct type *)

#define TB_PROTOTYPE_INSERT(name, type, attr) \
    attr struct type *name##_TB_INSERT(struct name *, struct type *)

#define TB_PROTOTYPE_REMOVE(name, type, attr) \
    attr struct type *name##_TB_REMOVE(struct name *, struct type *)

#define TB_PROTOTYPE_REINSERT(name, type, attr) \
    attr struct type *name##_TB_REINSERT(struct name *, struct type *)

#define TB_GENERATE(name, type, field, cmp) \
    TB_GENERATE_INTERNAL(name, type, field, cmp,)

#define TB_GENERATE_STATIC(name, type, field, cmp) \
    TB_GENERATE_INTERNAL(name, type, field, cmp, __tbtree_unused static)

#define TB_GENERATE_INTERNAL(name, type, field, cmp, attr) \
    TB_GENERATE_MIN(name, type, field, attr) \
    TB_GENERATE_MAX(name, type, field, attr) \
    TB_GENERATE_PREV(name, type, field, attr) \
    TB_GENERATE_NEXT(name, type, field, attr) \
    TB_GENERATE_FIRST(name, type, attr) \
    TB_GENERATE_LAST(name, type, attr) \
    TB_GENERATE_FIND(name, type, field, cmp, attr) \
    TB_GENERATE_NFIND(name, type, field, cmp, attr) \
    TB_GENERATE_INSERT(name, type, field, cmp, attr) \
    TB_GENERATE_REMOVE(name, type, field, cmp, attr) \
    TB_GENERATE_REINSERT(name, type, field, cmp, attr) \

#define TB_GENERATE_MIN(name, type, field, attr) \
    attr struct type *name##_TB_MIN(struct type *elm) { \
        while (!TB_LLEAF(elm, field)) \
            elm = TB_LEFT(elm, field); \
        return elm; \
    }

#define TB_GENERATE_MAX(name, type, field, attr) \
    attr struct type *name##_TB_MAX(struct type *elm) { \
        while (!TB_RLEAF(elm, field)) \
            elm = TB_RIGHT(elm, field); \
        return elm; \
    }

#define TB_GENERATE_PREV(name, type, field, attr) \
    attr struct type *name##_TB_PREV(struct type *elm) { \
        if (TB_LLEAF(elm, field)) \
            return TB_LEFT(elm, field); \
        return TB_MAX(name, TB_LEFT(elm, field)); \
    }

#define TB_GENERATE_NEXT(name, type, field, attr) \
    attr struct type *name##_TB_NEXT(struct type *elm) { \
        if (TB_RLEAF(elm, field)) \
            return TB_RIGHT(elm, field); \
        return TB_MIN(name, TB_RIGHT(elm, field)); \
    }

#define TB_GENERATE_FIRST(name, type, attr) \
    attr struct type *name##_TB_FIRST(struct name *head) { \
        return TB_EMPTY(head) ? NULL : TB_MIN(name, TB_ROOT(head)); \
    }

#define TB_GENERATE_LAST(name, type, attr) \
    attr struct type *name##_TB_LAST(struct name *head) { \
        return TB_EMPTY(head) ? NULL : TB_MAX(name, TB_ROOT(head)); \
    }

#define TB_GENERATE_FIND(name, type, field, cmp, attr) \
    attr struct type *name##_TB_FIND(struct name *head, struct type *elm) { \
        struct type *tmp = TB_ROOT(head); \
        while (tmp) { \
            int comp = (cmp)(elm, tmp); \
            if (comp < 0) { \
                if (TB_LLEAF(tmp, field)) \
                    return NULL; \
                tmp = TB_LEFT(tmp, field); \
            } else if (comp > 0) { \
                if (TB_RLEAF(tmp, field)) \
                    return NULL; \
                tmp = TB_RIGHT(tmp, field); \
            } else { \
                return tmp; \
            } \
        } \
        return NULL; \
    }

#define TB_GENERATE_NFIND(name, type, field, cmp, attr) \
    attr struct type *name##_TB_NFIND(struct name *head, struct type *elm) { \
        struct type *tmp = TB_ROOT(head); \
        struct type *res = NULL; \
        while (tmp) { \
            int comp = (cmp)(elm, tmp); \
            if (comp < 0) { \
                if (TB_LLEAF(tmp, field)) \
                    return tmp; \
                res = tmp; \
                tmp = TB_LEFT(tmp, field); \
            } else if (comp > 0) { \
                if (TB_RLEAF(tmp, field)) \
                    return NULL; \
                tmp = TB_RIGHT(tmp, field); \
            } else { \
                return tmp; \
            } \
        } \
        return res; \
    }

#define TB_GENERATE_INSERT(name, type, field, cmp, attr) \
    attr struct type *name##_TB_INSERT(struct name *head, struct type *elm) { \
        struct type *parent = NULL; \
        struct type *lprev = NULL; \
        struct type *rprev = NULL; \
        struct type **prev = &TB_ROOT(head); \
        struct type *tmp = *prev; \
        while (tmp) { \
            int comp = (cmp)(elm, tmp); \
            if (comp < 0) { \
                if (TB_LLEAF(tmp, field)) { \
                    TB_UP(elm, field) = TB_RIGHT(elm, field) = tmp; \
                    TB_SET(elm, field); \
                    TB_LEFT(elm, field) = TB_LEFT(tmp, field); \
                    TB_LEFT(tmp, field) = elm; \
                    TB_LCLEAR(tmp, field); \
                    return NULL; \
                } \
                rprev = parent = tmp; \
                prev = &TB_LEFT(tmp, field); \
                tmp = *prev; \
            } else if (comp > 0) { \
                if (TB_RLEAF(tmp, field)) { \
                    TB_UP(elm, field) = TB_LEFT(elm, field) = tmp; \
                    TB_SET(elm, field); \
                    TB_RIGHT(elm, field) = TB_RIGHT(tmp, field); \
                    TB_RIGHT(tmp, field) = elm; \
                    TB_RCLEAR(tmp, field); \
                    return NULL; \
                } \
                lprev = parent = tmp; \
                prev = &TB_RIGHT(tmp, field); \
                tmp = *prev; \
            } else { \
                TB_UP(elm, field) = NULL; \
                TB_SET(elm, field); \
                TB_LEFT(elm, field) = NULL; \
                TB_RIGHT(elm, field) = NULL; \
                return tmp; \
            } \
        } \
        TB_UP(elm, field) = parent; \
        TB_SET(elm, field); \
        TB_LEFT(elm, field) = lprev; \
        TB_RIGHT(elm, field) = rprev; \
        *prev = elm; \
        return NULL; \
    }

#define TB_REMOVE_LEAF(type, head, elm, field) do { \
        struct type *parent = TB_PARENT(elm, field); \
        if (!parent) { \
            TB_ROOT(head) = NULL; \
        } else if (TB_LEFT(parent, field) == (elm)) { \
            TB_LEFT(parent, field) = TB_LEFT(elm, field); \
            TB_LSET(parent, field); \
        } else { \
            TB_RIGHT(parent, field) = TB_RIGHT(elm, field); \
            TB_RSET(parent, field); \
        } \
    } while (0)

#define TB_REMOVE_LLEAF(name, type, head, elm, field) do { \
        struct type *child = TB_RIGHT(elm, field); \
        struct type *parent = TB_PARENT(elm, field); \
        TB_SWAP_CHILD(head, parent, elm, child, field); \
        TB_SET_PARENT(child, parent, field); \
    } while (0)

#define TB_REMOVE_RLEAF(name, type, head, elm, field) do { \
        struct type *child = TB_LEFT(elm, field); \
        struct type *parent = TB_PARENT(elm, field); \
        TB_SWAP_CHILD(head, parent, elm, child, field); \
        TB_SET_PARENT(child, parent, field); \
    } while (0)

#define TB_GENERATE_REMOVE(name, type, field, cmp, attr) \
    attr struct type *name##_TB_REMOVE(struct name *head, struct type *elm) { \
        if (TB_LEAF(elm, field)) { \
            TB_REMOVE_LEAF(type, head, elm, field); \
        } else if (TB_LLEAF(elm, field)) { \
            TB_LEFT(TB_NEXT(name, elm), field) = TB_LEFT(elm, field); \
            TB_REMOVE_LLEAF(name, type, head, elm, field); \
        } else if (TB_RLEAF(elm, field)) { \
            TB_RIGHT(TB_PREV(name, elm), field) = TB_RIGHT(elm, field); \
            TB_REMOVE_RLEAF(name, type, head, elm, field); \
        } else { \
            struct type *tmp = TB_NEXT(name, elm); \
            TB_RIGHT(TB_PREV(name, elm), field) = tmp; \
            if (TB_LEAF(tmp, field)) { \
                TB_REMOVE_LEAF(type, head, tmp, field); \
            } else { \
                TB_REMOVE_LLEAF(name, type, head, tmp, field); \
            } \
            struct type *parent = TB_PARENT(elm, field); \
            TB_SWAP_CHILD(head, parent, elm, tmp, field); \
            TB_UP(tmp, field) = parent; \
            TB_LEFT(tmp, field) = TB_LEFT(elm, field); \
            if (!TB_RIGHT(tmp, field)) { \
                TB_RSET(tmp, field); \
            } else { \
                TB_LEFT(TB_NEXT(name, tmp), field) = tmp; \
            } \
        } \
        return elm; \
    }

#define TB_GENERATE_REINSERT(name, type, field, cmp, attr) \
    attr struct type *name##_TB_REINSERT(struct name *head, struct type *elm) { \
        struct type *tmp; \
        if (((tmp = TB_PREV(name, elm)) && (cmp)(tmp, elm) >= 0) || \
            ((tmp = TB_NEXT(name, elm)) && (cmp)(elm, tmp) >= 0)) { \
            TB_REMOVE(name, head, elm); \
            return TB_INSERT(name, head, elm); \
        } \
        return NULL; \
    }

#define TB_MIN(name, ...)           name##_TB_MIN(__VA_ARGS__)
#define TB_MAX(name, ...)           name##_TB_MAX(__VA_ARGS__)
#define TB_PREV(name, ...)          name##_TB_PREV(__VA_ARGS__)
#define TB_NEXT(name, ...)          name##_TB_NEXT(__VA_ARGS__)
#define TB_FIRST(name, ...)         name##_TB_FIRST(__VA_ARGS__)
#define TB_LAST(name, ...)          name##_TB_LAST(__VA_ARGS__)
#define TB_FIND(name, ...)          name##_TB_FIND(__VA_ARGS__)
#define TB_NFIND(name, ...)         name##_TB_NFIND(__VA_ARGS__)
#define TB_INSERT(name, ...)        name##_TB_INSERT(__VA_ARGS__)
#define TB_REMOVE(name, ...)        name##_TB_REMOVE(__VA_ARGS__)
#define TB_REINSERT(name, ...)      name##_TB_REINSERT(__VA_ARGS__)

#define TB_FOREACH(var, name, head) \
    for ((var) = TB_FIRST(name, head); \
         (var); \
         (var) = TB_NEXT(name, var))

#define TB_FOREACH_FROM(var, name, head) \
    for ((var) = ((var) ? (var) : TB_FIRST(name, head)); \
         (var); \
         (var) = TB_NEXT(name, var))

#define TB_FOREACH_SAFE(var, name, head, tvar) \
    for ((var) = TB_FIRST(name, head); \
         (var) && ((tvar) = TB_NEXT(name, var), 1); \
         (var) = (tvar))

#define TB_FOREACH_FROM_SAFE(var, name, head, tvar) \
    for ((var) = ((var) ? (var) : TB_FIRST(name, head)); \
         (var) && ((tvar) = TB_NEXT(name, var), 1); \
         (var) = (tvar))

#define TB_FOREACH_REVERSE(var, name, head) \
    for ((var) = TB_LAST(name, head); \
         (var); \
         (var) = TB_PREV(name, var))

#define TB_FOREACH_REVERSE_FROM(var, name, head) \
    for ((var) = ((var) ? (var) : TB_LAST(name, head)); \
         (var); \
         (var) = TB_PREV(name, var))

#define TB_FOREACH_REVERSE_SAFE(var, name, head, tvar) \
    for ((var) = TB_LAST(name, head); \
         (var) && ((tvar) = TB_PREV(name, var), 1); \
         (var) = (tvar))

#define TB_FOREACH_REVERSE_FROM_SAFE(var, name, head, tvar) \
    for ((var) = ((var) ? (var) : TB_LAST(name, head)); \
         (var) && ((tvar) = TB_PREV(name, var), 1); \
         (var) = (tvar))
