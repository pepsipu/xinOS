#include <stddef.h>
#include <stdint.h>
#include <sys/apic.h>
#include <sys/cpu.h>

int init_apic()
{
    // check if apic is available
    uint32_t edx;
    cpuid(1, NULL, NULL, NULL, &edx);
    if (!(edx & (1 << 9)))
        return 1;
    return 0;
}