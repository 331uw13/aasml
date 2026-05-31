#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <math.h>

#include "codegen.h"


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
    { "rax", "eax", "ax",  "al" },
    { "rbx", "ebx", "bx",  "bl" },
    { "rcx", "ecx", "cx",  "cl" },
    { "rdx", "edx", "dx",  "dl" },
    { "rsi", "esi", "si",  "sil" },
    { "rdi", "edi", "di",  "dil" },
    { "rsp", "esp", "sp",  "spl" },
    { "rbp", "ebp", "bp",  "bpl" },
    { "r8",  "r8d", "r8w", "r8b"  },
    { "r9",  "r9d", "r9w", "r9b"  },
    { "r10", "r10d","r10w","r10b" },
    { "r12", "r12d","r12w","r12b" },
    { "r12", "r12d","r12w","r12b" },
    { "r13", "r13d","r13w","r13b" },
    { "r14", "r14d","r14w","r14b" },
    { "r15", "r15d","r15w","r15b" }
};

const char* get_suitable_register_width(char* most_wide_register, VariableSize size) {

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

    return most_wide_register;
}

/*
static
void p_generate_instructions_for_add_or_sub(StackFrame* stackframe, const char* instruction, IRcmd* cmd) {
    StackFrameVarInfo* var_info = stackframe_get(stackframe, cmd->as.op.lhs_name);

    char asm_mnemonic_lhs[8] = { 0 };

    if(is_text_number_literal(cmd->as.op.rhs_name)) {
        snprintf(asm_mnemonic_lhs, sizeof(asm_mnemonic_lhs)-1, 
                "$%s", cmd->as.op.rhs_name);
    }
    else {
        StackFrameVarInfo* rhs_var_info
            = stackframe_get(stackframe, cmd->as.op.rhs_name);

        const char* tmp_register 
            = get_suitable_register_width("rax", rhs_var_info->size);

        cg_printf
        (
            "\tmov%s -%d(%%rbp), %%%s\n",
            rhs_var_info->size.asm_spec,
            rhs_var_info->offset,
            tmp_register
        );
        snprintf(asm_mnemonic_lhs, sizeof(asm_mnemonic_lhs)-1, "%%%s", tmp_register);
    }

    cg_printf
    (
        "\t%s%s %s, -%d(%%rbp)\n",
        instruction,
        var_info->size.asm_spec,
        asm_mnemonic_lhs,
        var_info->offset
    );
}

static
void p_generate_instructions_for_mul(StackFrame* stackframe, IRcmd* cmd) {
   


}
*/

#define ASM_INS_MOV "mov"
#define ASM_INS_ADD "add"
#define ASM_INS_SUB "sub"

static
void p_asm_move_operand_to_register(const char* instruction, StackFrame* stackframe, const Operand* operand, const char* most_wide_register) {
    switch(operand->kind) { 
        
        case OPERAND_VARIABLE:
            {
                StackFrameVarInfo* operand_var_info = stackframe_get(stackframe, operand->text);

                cg_printf
                (
                    "\t%s%s -%d(%%rbp), %%%s\n",
                    instruction,
                    operand_var_info->size.asm_spec,
                    operand_var_info->offset,
                    get_suitable_register_width(most_wide_register, operand_var_info->size)
                );
            }
            break;

        case OPERAND_LITERAL_INT:
            {
                VariableSize var_size = get_variable_size(operand->type);
                cg_printf
                (
                    "\t%s%s $%s, %%%s\n",
                    instruction,
                    var_size.asm_spec,
                    operand->text,
                    get_suitable_register_width(most_wide_register, var_size)
                );
            }
            break;


        // ...

        default:
            fprintf(stderr, "%s: %s(): Operand kind is not handled in switch statement.\n",
                    __FILE__, __func__);
            break;
    }
}


static
void p_asm_move_operand_to_stack(const char* instruction, StackFrame* stackframe, const Operand* operand, const int offset) {
    switch(operand->kind) { 
        
        case OPERAND_VARIABLE:
            {
                StackFrameVarInfo* operand_var_info = stackframe_get(stackframe, operand->text);
                
                p_asm_move_operand_to_register(ASM_INS_MOV, stackframe, operand, "rax");

                cg_printf
                (
                    "\t%s%s %%%s, -%d(%%rbp)\n",
                    instruction,
                    operand_var_info->size.asm_spec,
                    get_suitable_register_width("rax", operand_var_info->size),
                    offset
                );
            }
            break;


        case OPERAND_LITERAL_INT:
            {
                VariableSize var_size = get_variable_size(operand->type);
                cg_printf
                (
                    "\t%s%s $%s, -%d(%%rbp)\n",
                    instruction,
                    var_size.asm_spec,
                    operand->text,
                    offset
                );
            }
            break;

        // ...

        default:
            fprintf(stderr, "%s: %s(): Operand kind is not handled in switch statement.\n",
                    __FILE__, __func__);
            break;
    }
}






void codegen_gnu_as_x86_64(TokenArray* token_array) {

    Token* tok = token_array->tokens;
    Token* tok_end = tok + token_array->count;
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


    while(tok < tok_end) {
        if(!tok->is_parsed) {
            tok++;
            continue;
        }

        switch(tok->kind) {

            case TOK_VAR:
                {
                    StackFrameVarInfo* var_info
                        = stackframe_add(&stackframe,
                                (StackFrameVarInfo) {
                                    .name = tok->parsed.as.operation.lhs.text,
                                    .size = get_variable_size
                                            (tok->parsed.as.operation.lhs.type)
                                });
                    
                    cg_printf_comment("'%s'\n", var_info->name);
                    p_asm_move_operand_to_stack(ASM_INS_MOV, &stackframe, &tok->parsed.as.operation.rhs, var_info->offset);
                }
                break;

            case TOK_FUNC:
                {
                    stackframe_free(&stackframe);
                    stackframe = stackframe_init_empty();

                    cg_printf("%s:\n", tok->parsed.as.function_impl.name);
                    cg_printf("\tpushq %%rbp\n");
                    cg_printf("\tmovq %%rsp, %%rbp\n");
                }
                break;

            case TOK_RET:
                {
                    cg_printf_comment("<return>\n");
                    p_asm_move_operand_to_register(ASM_INS_MOV, &stackframe, &tok->parsed.as.operation.rhs, "r8");
                    cg_printf("\tpopq %%rbp\n");
                    cg_printf("\tret\n");
                }
                break;


            case TOK_MOV:
                {
                    Operation* operation = &tok->parsed.as.operation;
                    StackFrameVarInfo* lhs_var_info 
                        = stackframe_get(&stackframe, operation->lhs.text);
                                
                    p_asm_move_operand_to_stack(ASM_INS_MOV, &stackframe, &operation->rhs, lhs_var_info->offset);
                }
                break;
        

            case TOK_ADD:
                {
                    Operation* operation = &tok->parsed.as.operation;
                    StackFrameVarInfo* lhs_var_info 
                        = stackframe_get(&stackframe, operation->lhs.text);
                                
                    p_asm_move_operand_to_stack(ASM_INS_ADD, &stackframe, &operation->rhs, lhs_var_info->offset);
                }
                break;

            case TOK_SUB:
                {
                    Operation* operation = &tok->parsed.as.operation;
                    StackFrameVarInfo* lhs_var_info 
                        = stackframe_get(&stackframe, operation->lhs.text);
                                
                    p_asm_move_operand_to_stack(ASM_INS_SUB, &stackframe, &operation->rhs, lhs_var_info->offset);
                }
                break;
        }

        tok++;
    }
                /*
    IRcmd* cmd = cmd_array->commands;
    IRcmd* end = cmd + cmd_array->count;


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
                            "\tmov%s -%d(%%rbp), %%%s # \"%s\"\n",
                            var_info->size.asm_spec,
                            var_info->offset,
                            get_suitable_register_width("r8", var_info->size),

                            // Comment
                            var_info->name
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
                */
}
