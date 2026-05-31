#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "codegen.h"

#include "../util.h"


VariableSize get_variable_size(VarType vartype) {
    switch(vartype) {
        case TYPE_BYTE:
            return (VariableSize) {
                .in_bytes = 1,
                .asm_spec = strdup("b")
            };
        
        case TYPE_INT16:
            return (VariableSize) {
                .in_bytes = 2,
                .asm_spec = strdup("w")
            };
        
        case TYPE_INT32:
            return (VariableSize) {
                .in_bytes = 4,
                .asm_spec = strdup("l")
            };
        
        case TYPE_INT64:
            return (VariableSize) {
                .in_bytes = 8,
                .asm_spec = strdup("q")
            };
    }


    // Should be unreachable due to previous stage's checks.
    fprintf(stderr, "%s: %s(): Variable type was not handled in the switch statement!\n",
            __FILE__, __func__);
    abort();
}

const char* REGISTERS[][4] = {
    { "rax", "eax", "ax", "al" },
    { "rbx", "ebx", "bx", "bl" },
    { "rcx", "ecx", "cx", "cl" },
    { "rdx", "edx", "dx", "dl" },
    { "rsi", "esi", "si", "sil" },
    { "rdi", "edi", "di", "dil" },
    { "rsp", "esp", "sp", "spl" },
    { "rbp", "ebp", "bp", "bpl" },
    { "r8",  "r8d", "r8w", "r8b"  },
    { "r9",  "r9d", "r9w", "r9b"  },
    { "r10", "r10d","r10w","r10b" },
    { "r12", "r12d","r12w","r12b" },
    { "r12", "r12d","r12w","r12b" },
    { "r13", "r13d","r13w","r13b" },
    { "r14", "r14d","r14w","r14b" },
    { "r15", "r15d","r15w","r15b" }
};

const char* get_suitable_register_by_size(char* most_wide_register, VariableSize size) {

    for(size_t i = 0; i < sizeof(REGISTERS) / sizeof *REGISTERS; i++) {
        if(strcmp(REGISTERS[i][0], most_wide_register) == 0) {
            int reg_index = 3 - (int)floor(log2f(size.in_bytes));
        
            /*
            printf("suitable register for %li bytes -----> %s [%d]\n",
                    size.in_bytes,
                    REGISTERS[i][reg_index],
                    reg_index);
                    */
            return REGISTERS[i][reg_index];
        }
    }

    return "?";
}


void codegen_gnu_as_x86_64(IRcmdArray* cmd_array) {
    IRcmd* cmd = cmd_array->commands;
    IRcmd* end = cmd + cmd_array->count;

    /*
    cg_printf(
        "global _start\n"
        "section .text:\n"
        "_start:\n"
        "   call entry\n"
        "   mov rax, 0x60\n"
        "   mov rdi, 0\n"
        "   int 0x80\n"
    );
    */


    StackFrame stackframe = stackframe_init_empty();


    cg_printf
    (
        ".global _start\n"
        ".text\n"
        "_start:\n"
        "\tcall entry\n"
        "\tmovq $60, %%rax\n"
        "\tmovq %%r8, %%rdi\n"
        "\tsyscall\n"
        "\n\n"
    );

    while(cmd < end) {
        switch(cmd->kind) {
        
            // Begin of function.
            case TOK_FUNC:
                {
                    stackframe_free(&stackframe);
                    stackframe = stackframe_init_empty();

                    cg_printf("%s:\n", cmd->as.func_impl.name);
                    cg_printf("\tpushq %%rbp\n");
                    cg_printf("\tmovq %%rsp, %%rbp\n");
                }
                break;

            case TOK_ADD:
                {
                }
                break;

            case TOK_VAR:
                {
                    StackFrameVarInfo* var_info
                        = stackframe_add(&stackframe,
                                (StackFrameVarInfo) {
                                    .name = cmd->as.new_var.name,
                                    .size = get_variable_size(cmd->as.new_var.type)
                                });

                    if(is_text_number_literal(cmd->as.new_var.init_val)) {
                        cg_printf
                        (
                            "\tmov%s $%s, -%d(%%rbp) # \"%s\"\n",
                            var_info->size.asm_spec,
                            cmd->as.new_var.init_val,
                            var_info->offset,

                            // Comment.
                            var_info->name
                        );
                    }
                    else {

                        // TODO
                    }
                }
                break;

            case TOK_RET:
                {


                    if(is_text_number_literal(cmd->as.ret.val)) {
                        
                        // TODO

                    }
                    else {
                        StackFrameVarInfo* var_info = stackframe_get(&stackframe, cmd->as.ret.val);

                        cg_printf
                        (
                            "\tmov%s -%d(%%rbp), %%%s\n",
                            var_info->size.asm_spec,
                            var_info->offset,
                            get_suitable_register_by_size("r8", var_info->size)
                        );
                    }

                    cg_printf("\tpopq %%rbp\n");
                    cg_printf("\tret\n");
                }
                break;
            


            default:
                fprintf(stderr, "%s: %s(): (%s) was not handled in the switch statement!\n",
                        __FILE__, __func__, token_kind_to_str(cmd->kind));
                abort();
                break;
        }
        cmd++;
    }

    stackframe_free(&stackframe);
}
