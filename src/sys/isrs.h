#ifndef ISRS_H
#define ISRS_H

#include <stdint.h>

typedef struct
{
    uint32_t err_code;
    uint32_t ip;
    uint32_t cs;
    uint32_t flags;
} interrupt_frame_t;

#endif