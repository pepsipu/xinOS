bits 32

global gdt
global gdt.ptr

gdt:
    .null:
    dq 0x0
    .code:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
    .data:
    dw 0xffff
    dw 0x0
    db 0x0
    db 10011010b
    db 11001111b
    db 0x0
    .ptr:
    dw $ - gdt - 1
    dd gdt