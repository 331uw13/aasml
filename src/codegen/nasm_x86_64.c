#include "codegen.h"




void gen_entry_point_call() {
    cg_printf(
    "global _start\n"
    "section .text:\n"
    "_start:\n"
    "   call entry\n"
    "   mov rax, 0x60\n"
    "   mov rdi, 0\n"
    "   int 0x80"
    );
}

