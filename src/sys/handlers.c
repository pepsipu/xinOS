#include <sys/cpu.h>
#include <sys/isrs.h>

__attribute__((interrupt)) void null_handler(interrupt_frame_t *frame) {
    // using this handler for this dummy p*psipu hasn't made yet
}