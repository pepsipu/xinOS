/*
    crypto_low.c
    a low level cryptographic interface for xinOS.
    WARNING:
    DO NOT USE IN LIFE OR DEATH SITUATIONS.
*/

#include <stdint.h>

// variables for xtea
#define XTEA_ROUNDS 64
#define DELTA 0x9e3779b9
// variables for speck
#define ROR(x, r) ((x >> r) | (x << (64 - r)))
#define ROL(x, r) ((x << r) | (x >> (64 - r)))
#define R(x, y, k) (x = ROR(x, 8), x += y, x ^= k, y = ROL(y, 3), y ^= x)
#define SPECK_ROUNDS 32

void encipher_xtea(uint32_t v[2], uint32_t const key[4])
{
    unsigned int i;
    uint32_t v0 = v[0], v1 = v[1], sum = 0;
    for (i = 0; i < XTEA_ROUNDS; i++)
    {
        v0 += (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
        sum += DELTA;
        v1 += (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}

void decipher_xtea(uint32_t v[2], uint32_t const key[4])
{
    unsigned int i;
    uint32_t v0 = v[0], v1 = v[1], sum = DELTA * XTEA_ROUNDS;
    for (i = 0; i < XTEA_ROUNDS; i++)
    {
        v1 -= (((v0 << 4) ^ (v0 >> 5)) + v0) ^ (sum + key[(sum >> 11) & 3]);
        sum -= DELTA;
        v0 -= (((v1 << 4) ^ (v1 >> 5)) + v1) ^ (sum + key[sum & 3]);
    }
    v[0] = v0;
    v[1] = v1;
}

void encipher_speck(uint64_t pt[2], uint64_t K[2])
{
    uint64_t y = pt[0], x = pt[1], b = K[0], a = K[1];

    R(x, y, b);
    for (int i = 0; i < SPECK_ROUNDS - 1; i++)
    {
        R(a, b, i);
        R(x, y, b);
    }

    pt[0] = y;
    pt[1] = x;
}