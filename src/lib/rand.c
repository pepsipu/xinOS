#include <stdint.h>
static uint64_t next = 1;

uint32_t rand(void)
{
    next = next * 1103515245 + 12345;
    return (uint32_t)(next / 65536) % 2147483647;
}

void srand(uint32_t seed)
{
    next = seed;
}