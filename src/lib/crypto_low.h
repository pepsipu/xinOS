#include <stdint.h>

void encipher_xtea(uint32_t v[2], uint32_t const key[4]);
void decipher_xtea(uint32_t v[2], uint32_t const key[4]);
void encipher_speck(uint64_t pt[2], uint64_t K[2]);