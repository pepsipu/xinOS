#include <lib/dynll.h>
#include <net/rtl8139.h>
#include <sys/pci.h>

#define RT_VENDOR 0x10ec
#define RT_DEVICE 0x8139

pci_function_t *pci_rtl8139 = 0;

pci_function_t *get_rtl8139()
{
    return search_dynll(pci_functions, element->device == RT_DEVICE && element->vendor == RT_VENDOR);
}

int init_rtl8139()
{
    pci_rtl8139 = get_rtl8139();
    if (!pci_rtl8139)
    {
        return 1;
    }
}