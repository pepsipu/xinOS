#include <stdint.h>

#define pcic_readw(bus, slot, func, offset) (uint16_t)((pcic_readd((bus), (slot), (func), (offset)) >> (((offset) & 2) * 8)) & 0xffff)
uint16_t pcic_readd(uint8_t bus, uint8_t slot, uint8_t func, uint8_t offset);
