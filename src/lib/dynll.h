#include <stddef.h>
#include <stdint.h>

// k level circular doubly linked list for generic use

#define new_dynll(name, type)                                                                                          \
    struct name##_dynll_node                                                                                           \
    {                                                                                                                  \
        struct name##_dynll_node *fd;                                                                                  \
        struct name##_dynll_node *bk;                                                                                  \
        type data;                                                                                                     \
    };                                                                                                                 \
    typedef struct name##_dynll_node name##_dynll_node_t;                                                              \
    name##_dynll_node_t name##_dynll = {&name##_dynll, &name##_dynll}

#define prepend_dynll(name, element)                                                                                   \
    ({                                                                                                                 \
        name##_dynll_node_t *new_node = kmalloc(sizeof(name##_dynll_node_t));                                          \
        new_node->data = element;                                                                                      \
        /* point old fd's bk at new node */                                                                            \
        name##_dynll.fd->bk = new_node;                                                                                \
        /* point new node's fd at head's old fd */                                                                     \
        new_node->fd = name##_dynll.fd;                                                                                \
        /* point new node's bk at head */                                                                              \
        new_node->bk = &name##_dynll;                                                                                  \
        /* point head's fd at new node */                                                                              \
        name##_dynll.fd = new_node;                                                                                    \
    })

#define append_dynll(name, element)                                                                                    \
    ({                                                                                                                 \
        name##_dynll_node_t *new_node = kmalloc(sizeof(name##_dynll_node_t));                                          \
        new_node->data = element;                                                                                      \
        /* point old bk's fd at new node */                                                                            \
        name##_dynll.bk->fd = new_node;                                                                                \
        /* point new node's bk at head's old bk */                                                                     \
        new_node->bk = name##_dynll.bk;                                                                                \
        /* point new node's fd at head */                                                                              \
        new_node->fd = &name##_dynll;                                                                                  \
        /* point head's bk at new node */                                                                              \
        name##_dynll.bk = new_node;                                                                                    \
    })

#define search_dynll_ptr(name, condition)                                                                              \
    ({                                                                                                                 \
        name##_dynll_node_t *ptr;                                                                                      \
        for (ptr = name##_dynll.fd; ptr != &name##_dynll; ptr = ptr->fd)                                               \
        {                                                                                                              \
            if (condition)                                                                                             \
                goto ret;                                                                                              \
        }                                                                                                              \
        ptr = 0;                                                                                                       \
    ret:                                                                                                               \
        ptr;                                                                                                           \
    })

#define iter_dynll(name, cb)                                                                                           \
    ({                                                                                                                 \
        name##_dynll_node_t *ptr;                                                                                      \
        for (ptr = name##_dynll.fd; ptr != &name##_dynll; ptr = ptr->fd)                                               \
        {                                                                                                              \
            cb(&ptr->data);                                                                                            \
        }                                                                                                              \
    })