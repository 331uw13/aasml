#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "codegen.h"

#include "../util.h"


/*
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
*/


/*
int get_vartype_size_in_bytes(VarType vartype) {
    switch(vartype) {
        case TYPE_BYTE: return 1;
        case TYPE_INT16: return 2;
        case TYPE_INT32: return 4;
        case TYPE_INT64: return 8;
    }

    // Should be unreachable due to previous stage's checks.
    return 0;
}

char* get_vartype_data_size_spec(VarType vartype) {
    switch(vartype) {
        case TYPE_BYTE: return strdup("BYTE");
        case TYPE_INT16: return strdup("WORD");
        case TYPE_INT32: return strdup("DWORD");
        case TYPE_INT64: return strdup("QWORD");
    }

    // Should be unreachable due to previous stage's checks.
    return 0;
}
*/
VariableSize get_variable_size(VarType vartype) {
    switch(vartype) {
        case TYPE_BYTE:
            return (VariableSize) {
                .in_bytes = 1,
                .asm_spec = strdup("BYTE")
            };
        
        case TYPE_INT16:
            return (VariableSize) {
                .in_bytes = 2,
                .asm_spec = strdup("WORD")
            };
        
        case TYPE_INT32:
            return (VariableSize) {
                .in_bytes = 4,
                .asm_spec = strdup("DWORD")
            };
        
        case TYPE_INT64:
            return (VariableSize) {
                .in_bytes = 8,
                .asm_spec = strdup("QWORD")
            };
    }


    // Should be unreachable due to previous stage's checks.
    fprintf(stderr, "%s: %s(): Variable type was not handled in the switch statement!\n",
            __FILE__, __func__);
    abort();
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
                            "\tmovl $%s, -%d(%%rbp)\n",
                            cmd->as.new_var.init_val,
                            var_info->offset
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
                        cg_printf("\tmovq $%s, %%r8\n", cmd->as.ret.val);
                    }
                    else {

                        // TODO
                    
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
