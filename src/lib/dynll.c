#include <stdint.h>
#include <stddef.h>

// k level circular doubly linked list for generic use

#define new_dynll(name, type) \
    struct name##_dynll_node \
    { \
        struct name##_dynll_node *fd; \
        struct name##_dynll_node *bk; \
        type data; \
    }; \
    typedef struct name##_dynll_node name##_dynll_node_t; \
    name##_dynll_node_t name##_dynll = { &name##_dynll, &name##_dynll }; \

#define prepend_dynll(name, element) ({ \
    name##_dynll_node_t *new_node = kmalloc(sizeof(name##_dynll_node_t)); \
    new_node->data = element; \
    new_node->fd = name##_dynll.fd; \
    name##_dynll.fd = new_node; \
)} \

#define append_dynll(name, element) ({ \
    name##_dynll_node_t *new_node = kmalloc(sizeof(name##_dynll_node_t)); \
    new_node->data = element; \
    new_node->bk = name##_dynll.bk; \
    name##_dynll.bk = new_node; \
)} \