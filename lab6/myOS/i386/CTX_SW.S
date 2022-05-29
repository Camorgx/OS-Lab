.text
.code32
.global CTX_SW
CTX_SW:
    pushf
    pusha
    movl prevTSK_StackPtrAddr, %eax
    movl (%eax), %ebx
    cmp %ebx, 0x0
    jz to_nxt
    movl %esp, (%eax)
    to_nxt: movl nextTSK_StackPtr, %esp
    popa
    popf
    ret