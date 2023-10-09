#include <sys/isrs.h>

__attribute__((interrupt)) void null_handler(interrupt_frame_t *frame);