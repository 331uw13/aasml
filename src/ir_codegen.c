#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <assert.h>

#include "compiler.h"




#define IRGEN_IGNORE_TOKEN_KINDS \
    case TOK_OC_PAREN: \
    case TOK_CC_PAREN: \
    case TOK_O_PAREN: \
    case TOK_C_PAREN: \
    case TOK_SEMICOLON: \
    case TOK_COLON: \
    case TOK_COMMA: \
    case TOK_L_ARROW: \
    case TOK_BYTE: \
    case TOK_INT32: \
    case TOK_INT64: \
    case TOK_LITERAL: \
    case TOK_IDENTIFIER: 



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


/*
static
IRcmdKind p_token_kind_to_ircmd_op_kind(TokenKind kind) {
    switch(kind) {
        case TOK_MOV: return IR_OP_MOV;
        case TOK_ADD: return IR_OP_ADD;
        case TOK_SUB: return IR_OP_SUB;
        case TOK_MUL: return IR_OP_MUL;
        case TOK_DIV: return IR_OP_DIV;
    }

    fprintf(stderr, "%s: %s(): Token (%s) does not represent a mnemonic!\n",
            __FILE__, __func__, token_kind_to_str(kind));
    abort();
}

const char* ircmd_kind_to_str(IRcmdKind kind) {
    switch(kind) {
        case IR_FUNC_IMPL: return "IR_FUNC_IMPL";
        case IR_NEW_VAR: return "IR_NEW_VAR";
        case IR_OP_MOV: return "IR_OP_MOV";
        case IR_OP_ADD: return "IR_OP_ADD";
        case IR_OP_SUB: return "IR_OP_SUB";
        case IR_OP_MUL: return "IR_OP_MUL";
        case IR_OP_DIV: return "IR_OP_DIV";
    }

    fprintf(stderr, "%s: %s(): Warning: some ircmd kind does not return its type in string format.\n",
            __FILE__, __func__);
    return "IR__UNKNOWN__";
}
*/



IRcmdArray* intermediate_codegen(TokenArray* token_array) {
    IRcmdArray* ircmds = malloc(sizeof *ircmds);
    ircmds->commands = NULL;
    ircmds->count = 0;
    ircmds->allocated_count = 0;





    Token* token = token_array->tokens;
    Token* tokens_end = token + token_array->count;


    while(token < tokens_end) {

        int num_skip = 0;

        IRcmd cmd = {0};
        cmd.kind = token->kind;


        switch(token->kind) {


            /*
            case TOK_FUNC:
                {
                    Token* tok_name_ident = token + 1;
                    cmd.as.func_impl.name = strdup(tok_name_ident->text);

                    add_ircmd(ircmds, cmd);
                    num_skip = 1;
                }
                break;

            case TOK_RET:
                {
                    Token* tok_name_ident = token + 1;
                    
                    cmd.as.ret.val = strdup(tok_name_ident->text);
                    add_ircmd(ircmds, cmd);
                
                    num_skip = 1;
                }
                break;
                */
    
            /*
            case TOK_RET:
                {
                    Token* tok_name_ident = token + 1;
                    
                    cmd.as.ret.val = strdup(tok_name_ident->text);
                    add_ircmd(ircmds, cmd);
                
                    num_skip = 1;
                }
                break;

            case TOK_FUNC:
                {
                    Token* tok_name_ident = token + 1;
                    cmd.as.func_impl.name = strdup(tok_name_ident->text);

                    add_ircmd(ircmds, cmd);
                    num_skip = 1;
                }
                break;

            case TOK_VAR:
                {
                    Token* tok_name_ident = token + 1;
                    Token* tok_type       = token + 3;
                    Token* tok_init_val   = token + 5; 

                    cmd.as.new_var.name = strdup(tok_name_ident->text);
                    cmd.as.new_var.type = token_kind_to_vartype(tok_type->kind);
                    cmd.as.new_var.init_val = strdup(tok_init_val->text);

                    add_ircmd(ircmds, cmd);
                    num_skip = 4;
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

                    //cmd.kind = p_token_kind_to_ircmd_op_kind(token->kind);
                    cmd.as.op.lhs_name = strdup(tok_lhs_name->text);
                    cmd.as.op.rhs_name = strdup(tok_rhs_name->text);

                    add_ircmd(ircmds, cmd);
                    num_skip = 2;
                }
                break;
                */

                IRGEN_IGNORE_TOKEN_KINDS
                break;

            default:
                fprintf(stderr, "%s: %s(): Token (%s) is not handled in switch statement!\n",
                        __FILE__, __func__, token_kind_to_str(token->kind));
                abort();
                break;
        }


        token += 1 + num_skip;
    }



    return ircmds;
}
