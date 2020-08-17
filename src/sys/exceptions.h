#include <sys/isrs.h>

__attribute__((interrupt)) void exc_double_fault(interrupt_frame_t *frame);