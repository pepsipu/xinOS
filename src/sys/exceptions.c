#include <lib/kprint.h>
#include <sys/cpu.h>
#include <sys/exceptions.h>
#include <sys/isrs.h>

// char *exception_messages[32] = {"DIVISION_BY_ZERO",
//                                 "DEBUG",
//                                 "NON-MASKABLE_INTERRUPT",
//                                 "BREAKPOINT",
//                                 "DETECTED_OVERFLOW",
//                                 "OUT-OF-BOUNDS",
//                                 "INVALID_OPCODE",
//                                 "NO_COPROCESSOR",
//                                 "DOUBLE_FAULT",
//                                 "COPROCESSOR_SEGMENT_OVERRUN",
//                                 "BAD_TSS",
//                                 "SEGMENT_NOT_PRESENT",
//                                 "STACK_FAULT",
//                                 "GENERAL_PROTECTION_FAULT",
//                                 "PAGE_FAULT",
//                                 "UNKNOWN_INTERRUPT",
//                                 "COPROCESSOR_FAULT",
//                                 "ALIGNMENT_CHECK",
//                                 "MACHINE_CHECK",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED",
//                                 "RESERVED"};

__attribute__((interrupt)) void exc_double_fault(interrupt_frame_t *frame)
{
    kprint("xinOS has crashed!\n");
    kprint("error: DOUBLE_FAULT\n");
    kprint("flags: %x\n", frame->flags);
    kprint("ip: %x\n", frame->ip);
    kprint("cs: %x\n", frame->cs);

    cli();
    while (1)
    {
        hlt();
    }
}