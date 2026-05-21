#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "compiler.h"







void add_ircmd(IRcmdArray* ircmds, IRcmd cmd) {
    if(ircmds->count+1 >= ircmds->allocated_count) {
        ircmds->allocated_count = ircmds->count + 64;
        ircmds->commands
            = realloc
            (
                ircmds->commands,
                ircmds->allocated_count * sizeof *ircmds->commands
            );
    }


    ircmds->commands[ircmds->count] = cmd;
    ircmds->count += 1;
}


// NOTE: Temporary function.
void ircmd_print(IRcmd* ircmd) {
    switch(ircmd->kind) {
        case IR_FUNC_IMPL:
            printf("func '%s'\n", ircmd->as.func_impl.name);
            break;

        case IR_NEW_VAR:
            printf("new var '%s'\n", ircmd->as.new_var.name);
            break;

        case IR_OP_MOV:
            printf("move '%s' '%s'\n", ircmd->as.op.lhs_name, ircmd->as.op.rhs_name);
            break;
        
        case IR_OP_ADD:
            printf("add '%s' '%s'\n", ircmd->as.op.lhs_name, ircmd->as.op.rhs_name);
            break;

        case IR_OP_SUB:
            printf("sub '%s' '%s'\n", ircmd->as.op.lhs_name, ircmd->as.op.rhs_name);
            break;
        
        case IR_OP_MUL:
            printf("mul '%s' '%s'\n", ircmd->as.op.lhs_name, ircmd->as.op.rhs_name);
            break;
        
        case IR_OP_DIV:
            printf("div '%s' '%s'\n", ircmd->as.op.lhs_name, ircmd->as.op.rhs_name);
            break;
    }
}

// NOTE: Temporary function.
static
IRcmdKind p_token_kind_to_ircmd_op_kind(TokenKind kind) {
    switch(kind) {
        case TOK_MOV: return IR_OP_MOV;
        case TOK_ADD: return IR_OP_ADD;
        case TOK_SUB: return IR_OP_SUB;
        case TOK_MUL: return IR_OP_MUL;
        case TOK_DIV: return IR_OP_DIV;

    }
   
    assert(0 && "p_token_kind_to_ircmd_op_kind: Token kind not handled.");
}

IRcmdArray* intermediate_codegen(TokenArray* token_array) {
    IRcmdArray* ircmds = malloc(sizeof *ircmds);
    ircmds->commands = NULL;
    ircmds->count = 0;
    ircmds->allocated_count = 0;





    Token* token = token_array->tokens;
    Token* tokens_end = token + token_array->count;


    while(token < tokens_end) {

        int num_skip = 0;

        switch(token->kind) {
            case TOK_FUNC:
                {
                    Token* tok_name_ident = token + 1;
                    IRcmd cmd = (IRcmd) {
                        .kind = IR_FUNC_IMPL,
                        .as.func_impl.name = strdup(tok_name_ident->text)
                    };

                    add_ircmd(ircmds, cmd);
                    num_skip = 1;
                }
                break;

            case TOK_VAR:
                {
                    Token* tok_name_ident = token + 1;
                    Token* tok_type = token + 3;

                    IRcmd cmd = (IRcmd) {
                        .kind = IR_NEW_VAR,
                        .as.new_var.name = strdup(tok_name_ident->text),
                        .as.new_var.type = token_kind_to_vartype(tok_type->kind)
                    };

                    add_ircmd(ircmds, cmd);
                    num_skip = 2;
                }
                break;

            case TOK_MOV:
            case TOK_ADD:
            case TOK_SUB:
            case TOK_MUL:
            case TOK_DIV:
                {
                    Token* tok_lhs_name = token + 1;
                    Token* tok_rhs_name = token + 3;

                    IRcmd cmd = (IRcmd) {
                        .kind = p_token_kind_to_ircmd_op_kind(token->kind),
                        .as.op.lhs_name = strdup(tok_lhs_name->text),
                        .as.op.rhs_name = strdup(tok_rhs_name->text)
                    };

                    add_ircmd(ircmds, cmd);
                    num_skip = 2;
                }
                break;

        }


        token += 1 + num_skip;
    }



    return ircmds;
}
