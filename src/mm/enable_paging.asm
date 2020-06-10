global enable_paging:function (enable_paging.end - enable_paging)

enable_paging:
    mov eax, [esp + 4] ; load page dir
    mov cr3, eax
    
    mov eax, cr0
    or eax, 0x80000001
    mov cr0, eax
    ret
    .end: